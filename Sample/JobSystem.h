#pragma once
#include <functional>
#include <future>
#include <memory>
#include <vector>

using JobHandle = std::shared_future<void>;

/*
* @brief ジョブシステム
*/
class JobSystem
{
public:
	JobSystem() = delete;
	~JobSystem() = delete;
	/*
	* @brief ジョブシステムを初期化します
	* @param workerCount ワーカースレッド数
	*/
	static void initialize(size_t workerCount);
	/*
	* @brief ジョブシステムをシャットダウンします
	*/
	static void shutdown();

	/*
	* @brief ジョブをスケジュールします
	* @param job ジョブ関数
	* @return ジョブハンドル
	*/
	static JobHandle schedule(const std::function<void()>& job);

	/*
	* @brief 依存関係付きでジョブをスケジュールします
	* @param job ジョブ関数
	* @param dependencies 依存するジョブハンドルの配列
	* @return ジョブハンドル
	*/
	static JobHandle schedule(const std::function<void()>& job, const std::vector<JobHandle>& dependencies);

	/*
	* @brief 並列Forジョブをスケジュールします
	* @param job ジョブ関数（引数はインデックス）
	* @param size ジョブの総数
	* @param innerLoopBatchCount 内部ループのバッチ数
	* @return ジョブハンドル
	*/
	static JobHandle schedule(const std::function<void(size_t)>& job, size_t size, size_t innerLoopBatchCount);

	/*
	* @brief すべてのジョブの完了を待機します
	*
	* @param jobHandles ジョブハンドルの配列
	*/
	static void waitForAll(const std::vector<JobHandle>& jobHandles);

	/*
	* @brief スレッドIDを文字列として取得します
	*/
	static std::string getAsString(const std::thread::id& threadId);
private:
	class Impl;
	static std::unique_ptr<Impl> impl;
};

