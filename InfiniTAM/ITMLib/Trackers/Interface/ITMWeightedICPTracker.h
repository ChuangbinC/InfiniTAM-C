// Copyright 2014-2015 Isis Innovation Limited and the authors of InfiniTAM

#pragma once

#include "ITMTracker.h"
#include "../../Engines/LowLevel/Interface/ITMLowLevelEngine.h"
#include "../../Objects/Tracking/ITMImageHierarchy.h"
#include "../../Objects/Tracking/ITMSceneHierarchyLevel.h"
#include "../../Objects/Tracking/ITMTemplatedHierarchyLevel.h"
#include "../../Objects/Tracking/TrackerIterationType.h"

#include "../../../ORUtils/HomkerMap.h"
#include "../../../ORUtils/SVMClassifier.h"

namespace ITMLib
{
	/** Base class for engine performing ICP based depth tracking.
	    A typical example would be the original KinectFusion
	    tracking algorithm.
	*/
	class ITMWeightedICPTracker : public ITMTracker
	{
	private:
		const ITMLowLevelEngine *lowLevelEngine;
		ITMImageHierarchy<ITMSceneHierarchyLevel> *sceneHierarchy;
		ITMImageHierarchy<ITMTemplatedHierarchyLevel<ITMFloatImage> > *viewHierarchy;
		ITMImageHierarchy<ITMTemplatedHierarchyLevel<ITMFloatImage> > *weightHierarchy;

		ITMTrackingState *trackingState; const ITMView *view;

		int *noIterationsPerLevel;
		int noICPLevel;

		float terminationThreshold;

		float hessian[6 * 6];
		float nabla[6];
		float step[6];

		void PrepareForEvaluation();
		void SetEvaluationParams(int levelId);

		void ComputeDelta(float *delta, float *nabla, float *hessian, bool shortIteration) const;
		void ApplyDelta(const Matrix4f & para_old, const float *delta, Matrix4f & para_new) const;
		bool HasConverged(float *step) const;

		void SetEvaluationData(ITMTrackingState *trackingState, const ITMView *view);

		void UpdatePoseQuality(int noValidPoints_old, float *hessian_good, float f_old);

		ORUtils::HomkerMap *map;
		ORUtils::SVMClassifier *svmClassifier;
		Vector4f mu, sigma;
	protected:
		float *distThresh;

		int levelId;
		TrackerIterationType iterationType;

		Matrix4f scenePose;
		ITMSceneHierarchyLevel *sceneHierarchyLevel;
		ITMTemplatedHierarchyLevel<ITMFloatImage> *viewHierarchyLevel;
		ITMTemplatedHierarchyLevel<ITMFloatImage> *weightHierarchyLevel;

		virtual int ComputeGandH(float &f, float *nabla, float *hessian, Matrix4f approxInvPose) = 0;

	public:
		void TrackCamera(ITMTrackingState *trackingState, const ITMView *view);

		bool requiresColourRendering(void) const { return false; }
		bool requiresDepthReliability(void) const { return true; }

		ITMWeightedICPTracker(Vector2i imgSize, TrackerIterationType *trackingRegime, int noHierarchyLevels, int noICPRunTillLevel, float distThresh,
			float terminationThreshold, const ITMLowLevelEngine *lowLevelEngine, MemoryDeviceType memoryType);
		virtual ~ITMWeightedICPTracker(void);
	};
}

