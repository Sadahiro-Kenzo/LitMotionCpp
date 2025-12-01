# LitMotionCpp

[日本語版READMEはこちら](README_JA.md)

## Overview
This is a C++ port of [LitMotion](https://github.com/AnnulusGames/LitMotion).

LitMotion is a high-performance tween library for Unity.

Cut off exclusive spec for Unity. but animate anything.

## Features
* Animate anything in one line of code.
* Multi thread execution
* Supports complex settings like easing and looping
* Waits for completion using callbacks
* Type extension with IMotionOptions and IMotionAdapter

## Setup
### Requirements
* Visual Studio 2022
* C++20
### Install
1. Header files copy and add to the PATH.
    * AnimationCurve.h
    * EaseUtility.h
    * FastListCore.h
    * IJobParallelFor.h
    * IMotionAdapter.h
    * IMotionOptions.h
    * IntegerOptions.h
    * LMotion.h
    * ManualMotionDispatcher.h
    * MotionBuilder.h
    * MotionCallbackData.h
    * MotionData.h
    * MotionDispatcher.h
    * MotionHandle.h
    * MotionScheduler.h
    * MotionStatus.h
    * MotionStorage.h
    * MotionStorageManager.h
    * MotionUpdateJob.h
    * NoOptions.h
    * ParallelJobScheduler.h
    * PrimitiveMotionAdapter.h
    * UpdateRunner.h
2. Link LitMotionCpp.lib.
3. Include LMotion.h,you want to  add an animation.
4. Call MotionDispatcher::setRealSinceStartup(),top of main loop. And call MotionDispatcher::update() at timing,you want to update of animation.

## Getting Started
Using LitMotion allows easy animation of values such as Object' field. To create motion, use LMotion::create().

Here's a sample code. 

example.h
~~~cpp
#include "LMotion.h"

enum class PlayerLoopTiming
{
    Update=0,
    PostUpdate=1,
};

struct SpriteData
{
    int id;
    float x;
    float y;
};

class Example
{
private:
    LitMotionCpp::MotionScheduler m_scheduler;
    SpriteData m_target1;
    SpriteData m_target2;
    float m_value;
    bool m_completed;
public:
    Example();
    void setup();
};
~~~

example.cpp
~~~cpp
#include "example.h"

using namespace LitMotionCpp;

Example::Example()
    :m_scheduler(MotionScheduler::createPlayerLoop(PlayerLoopTiming::PostUpdate))
{

}

void Example::setup()
{
    LMotion::create(0.0f,1.0f,2.0f) //  Animate value from 0.0f to 1.0f over 2 seconds
        .bind<SpriteData>(&m_target1,[](float value, SpriteData* pState) { pState->x = value; });  //  Bind to m_target1.x

    LMotion::create(0.0f,10.0f,2.0f)    //  Animate value 0.0f to 10.0f over 2 seconds
        .withEase(Ease::OutQuad)         //  Specify easing function
        .withLoops(2, LoopType::Yoyo)   //  Specify loop count and type
        .withDelay(0.2f) // Set delay
        .bind<SpriteData>(&m_target2,[](float value, SpriteData* pState) { pState->x = value; });  //  Bind to m_target2.x

    m_completed=false;
    LMotion::create(0.0f,10.0f,2.0f)    //  Animate value 0.0f to 10.0f over 2 seconds
        .withScheduler(m_scheduler) //  Specify execution timing with Scheduler
        .withOnComplete([this](){m_completed=true;})    //  Set a callback
        .withCancelOnError()    //  Cancel motion if exception occurs within Bind
        .bind<Example>(this,[](float value,Example* pState){ pState->m_value=value;});  //  Pass the target pointer as the first argument

    //  Control created motions via the `MotionHandle` struct
    auto handle=LMotion::create(0.0f,1.0f,2.0f).runWithoutBinding();

    if(handle.isActive())   //  Returns true if the motion is playing
    {
        handle.cancel();    //  Cancel the motion
        handle.complete();  //  Complete the motion
    }
}
~~~
## Replacements of 'Unity' functions
### Call update and get update time
At Unity,update method is called by frame after register to PlayerLoop,and get update time form Time.

At 'LitMotionCpp',set update time at first of frame,and must to call update method in frame.
~~~cpp
void Game::Update(DX::StepTimer const& timer)
{
    //  Set begin time of frame
    MotionDispatcher::setRealtimeSinceStartup(timer.GetTotalSeconds());

    //  update motion data of default scheduler
    MotionDispatcher::update(MotionScheduler::getDefaultScheduler());
}
~~~
### AnimationCurve
AnimationCurve class Implement Bezier curve interpolation from key frame. Keyframe pass to withEase function. Keyframe is not copied,it only has pointer. Do not delete original,until animation end.
~~~cpp
using namespace LitMotionCpp;

const Keyframe keyframes[] = {
	{0.0f, 0.0f, 0.0f, 0.0f},
	{0.32483720779418948f, 0.9665907025337219f, 0.08513136953115463f, 0.08513136953115463f},
	{0.6239457130432129f, -0.027526676654815675f, 0.59372878074646f, 0.59372878074646f},
	{0.6815565824508667f, 1.0051050186157227f, -0.017104098573327066f, -0.017104098573327066f},
	{0.8800565600395203f, -0.01362311840057373f, -0.00554767856374383f, -0.00554767856374383f},
	{1.0f, 1.0f, 0.0f, 0.0f}
};

void Example::setup()
{
    LMotion::create(-5.0f, 5.0f, 3.0f)
        .withEase(std::begin(keyframes), std::end(keyframes))
        .bind<Example>(this,[](float value,Example* pState){pState->m_value=value;});
}
~~~
You can also replace it with an implementation that performs your own keyframe interpolation by registering a factory function.
~~~cpp
void Game::registerAnimationCurveFactory()
{
    AnimationCurve::setFactory([](const Keyframe* begin, const Keyframe* end, WrapMode preMode, WrapMode postMode) -> AnimationCurve* {
        return new DirectXMathAnimationCurve(begin,end,preMode,postMode);
		});
}
~~~
ref. Wikipedia
[Cubic Hermite spline](https://en.wikipedia.org/wiki/Cubic_Hermite_spline),
[Bézier curve](https://en.wikipedia.org/wiki/B%C3%A9zier_curve)
### DOTS
The default implementation updates on the main thread. By registering a function that executes the IJobParallelFor interface, updates can be made on multiple threads. The Sample project also implements a simple JobSystem.
~~~cpp
void Game::registerJobSystem()
{
    //  Launch 4 worker threads
	JobSystem::initialize(4);

	//  Job system startup function registration
    ParallelJobScheduler::setExecuter([](IJobParallelFor& job, size_t size, size_t innerLoopBatchCount){
        auto handle=JobSystem::schedule([&job](size_t index) {
			    job.execute(static_cast<int>(index));
			}, size, innerLoopBatchCount);
		handle.wait();
		});
}
~~~
ref. 
[C++でジョブシステムを作ってみる(1)](https://zenn.dev/nishiki/articles/01ff1417f0b85f)
### Sample Project
SampleScene implement as application,added menu scene.
* Select by cursor up/down key,and execute by spece key.
* Back to menu by ESC key after sample scene end.
* Space key redo sample scene.
![img](sample_menu.png)

## License

[MIT License](LICENSE)

## Dependent Libraries in Sample

[DirectXTK12](https://github.com/microsoft/DirectXTK12)

[Vecmath-cpp](https://github.com/yuki12/vecmath-cpp)
