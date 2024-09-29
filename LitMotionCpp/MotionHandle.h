#pragma once
namespace LitMotionCpp
{
	/**
	* @brief  An identifier that represents a specific motion entity.
	*/
	struct MotionHandle
	{
		MotionHandle() = default;
		MotionHandle(const MotionHandle&) = default;
		~MotionHandle() = default;

		/**
		* @brief The ID of motion storage.
		*/
		int StorageId;

		/**
		* @brief The ID of motion entity.
		*/
		int Index;

		/**
		* @brief The generational version of motion entity.
		*/
		int Version;

		/**
		* @brief Checks if a motion is currently playing.
		* 
		* @return True if motion is active, otherwise false. 
		*/
		bool isActive();

		/**
		* @brief Complete Motion
		*/
		void complete();

		/**
		* @brief Cancel Motion
		*/
		void cancel();

		bool operator == (const MotionHandle&);

		bool operator !=(const MotionHandle&);
	};

}//namespace

