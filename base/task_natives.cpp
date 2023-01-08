#include "fwanimation/animmanager.h"
#include "fwanimation/directorcomponentsyncedscene.h"
#include "fwscene/stores/networkdefstore.h"
#include "fwscene/stores/framefilterdictionarystore.h"
#include "script/commands_task.h"
#include "script/wrapper.h"
#include "script/thread.h"
//Game headers
#include "ai/debug/system/AIDebugLogManager.h"
#include "animation/AnimDefines.h"
#include "animation/Move.h"
#include "control/WaypointRecording.h"
#include "control/replay/replay.h"
#include "event/EventShocking.h"
#include "event/EventWeapon.h"
#include "event/ShockingEvents.h"
#include "game/ModelIndices.h"
#include "math/angmath.h"
#include "network/NetworkInterface.h"
#include "network/Events/NetworkEventTypes.h"
#include "peds/PedFactory.h"
#include "peds/PedHelmetComponent.h"
#include "peds/PedIntelligence.h"
#include "peds/Pedpopulation.h"
#include "peds/PedTaskRecord.h"
#include "peds/Ped.h"
#include "peds/WildlifeManager.h"
#include "physics/WorldProbe/worldprobe.h"
#include "physics/RagdollConstraints.h"
#include "pickups/Pickup.h"
#include "pickups/PickupPlacement.h"
#include "scene/world/GameWorld.h"
#include "fwpheffects/ropemanager.h"
#include "fwscene/search/SearchVolumes.h"
#include "script/Handlers/GameScriptEntity.h"
#include "script/Handlers/GameScriptResources.h"
#include "script/Handlers/GameScriptMgr.h"
#include "script/scrchannel.h"
#include "script/script.h"
#include "script/script_cars_and_peds.h"
#include "script/script_helper.h"
#include "script/ScriptTaskTypes.h"
#include "script/commands_waypoint.h"
#include "script/commands_entity.h"
#include "streaming/streaming.h"
#include "Task/Animation/TaskMoveScripted.h"
#include "Task/Combat/Cover/Cover.h"
#include "Task/Combat/TaskNewCombat.h"
#include "Task/Combat/Cover/TaskCover.h"
#include "Task/Combat/Cover/TaskSeekCover.h"
#include "Task/Combat/Cover/TaskStayInCover.h"
#include "Task/Combat/Subtasks/TaskDraggingToSafety.h"
#include "Task/Combat/Subtasks/TaskHeliChase.h"
#include "Task/Combat/Subtasks/TaskPlaneChase.h"
#include "Task/Combat/Subtasks/TaskStealth.h"
#include "Task/Combat/Subtasks/TaskVehicleChase.h"
#include "Task/Combat/Subtasks/TaskVehicleCombat.h"
#include "task/Combat/TaskCombat.h"
#include "task/Combat/TaskCombatMelee.h"
#include "Task/Combat/TaskDamageDeath.h"
#include "Task/Combat/TaskInvestigate.h"
#include "Task/Combat/TaskReact.h"
#include "Task/Combat/TaskSearch.h"
#include "Task/Combat/TaskSharkAttack.h"
#include "Task/Combat/TaskThreatResponse.h"
#include "Task/Combat/TaskWrithe.h"
#include "Task/Default/AmbientAnimationManager.h"
#include "Task/Default/Patrol/PatrolRoutes.h"
#include "Task/Default/Patrol/TaskPatrol.h"
#include "Task/Default/TaskArrest.h"
#include "Task/Default/ArrestHelpers.h"
#include "Task/Default/TaskChat.h"
#include "Task/Default/TaskCuffed.h"
#include "Task/Default/TaskPlayer.h"
#include "Task/Default/TaskWander.h"
#include "Task/General/TaskBasic.h"
#include "Task/General/TaskGeneralSweep.h"
#include "Task/General/Phone/TaskMobilePhone.h"
#include "Task/Scenario/info/ScenarioInfoManager.h"
#include "Task/General/TaskSecondary.h"
#include "Task/Motion/Locomotion/TaskMotionPed.h"
#include "Task/Movement/Climbing/TaskGoToAndClimbLadder.h"
#include "Task/Movement/Climbing/TaskRappel.h"
#include "Task/Movement/Jumping/TaskJump.h"
#include "Task/Movement/TaskFall.h"
#include "Task/Movement/TaskSlideToCoord.h"
#include "Task/Movement/TaskCollisionResponse.h"
#include "Task/Movement/TaskFollowWaypointRecording.h"
#include "Task/Movement/TaskGoto.h"
#include "Task/Movement/TaskGoToPointAiming.h"
#include "Task/Movement/TaskMoveFollowEntityOffset.h"
#include "Task/Movement/TaskParachute.h"
#include "Task/Movement/TaskJetpack.h"
#include "Task/Movement/TaskSeekEntity.h"
#include "Task/Physics/TaskAnimatedAttach.h"
#include "Task/Physics/TaskNM.h"
#include "Task/Physics/TaskNMBindPose.h"
#include "Task/Physics/TaskNMElectrocute.h"
#include "Task/Physics/TaskNMDangle.h"
#include "Task/Physics/TaskNMHighFall.h"
#include "Task/Physics/TaskNMSlungOverShoulder.h"
//#include "Task/Physics/TaskNMStumble.h"
#include "Task/Response/TaskAgitated.h"
#include "Task/Response/TaskConfront.h"
#include "Task/Response/TaskFlee.h"
#include "Task/Response/TaskGangs.h"
#include "task/Response/TaskReactAndFlee.h"
#include "Task/Response/TaskShockingEvents.h"
#include "Task/Scenario/Info/ScenarioInfo.h"
#include "Task/Scenario/ScenarioManager.h"
#include "Task/Scenario/ScenarioPointManager.h"
#include "Task/Scenario/Types/TaskParkedVehicleScenario.h"
#include "Task/Scenario/Types/TaskUseScenario.h"
#include "Task/Service/Police/TaskPolicePatrol.h"
#include "Task/Service/Swat/TaskSwat.h"
#include "Task/System/Task.h"
#include "Task/Animation/TaskAnims.h"
#include "Task/Animation/TaskScriptedAnimation.h"
#include "Task/System/TaskManager.h"
#include "Task/Vehicle/TaskCar.h"
#include "Task/Vehicle/TaskCarAccessories.h"
#include "Task/Vehicle/TaskCarUtils.h"
#include "Task/Vehicle/TaskEnterVehicle.h"
#include "Task/Vehicle/TaskExitVehicle.h"
#include "Task/Vehicle/TaskMountAnimal.h"
#include "Task/Vehicle/TaskRideTrain.h"
#include "Task/Vehicle/TaskVehicleWeapon.h"
#include "Task/Weapons/Gun/TaskGun.h"
#include "Task/Weapons/Gun/TaskAimGunScripted.h"
#include "Task/Weapons/Gun/TaskReloadGun.h"
#include "Task/Weapons/Gun/Metadata/ScriptedGunTaskInfo.h"
#include "Task/Weapons/Gun/Metadata/ScriptedGunTaskInfoMetadataMgr.h"
#include "Task/Weapons/Gun/TaskVehicleDriveBy.h"
#include "Task/Weapons/TaskBomb.h"
#include "Task/Weapons/TaskProjectile.h"
#include "Task/Weapons/TaskSwapWeapon.h"
#include "Task/Weapons/WeaponController.h"
#include "debug/DebugScene.h"
#include "vehicleAi/vehicleintelligence.h"
#include "vehicleAi/Task/TaskVehicleGoTo.h"
#include "vehicleAi/Task/TaskVehicleGotoAutomobile.h"
#include "vehicleAi/Task/TaskVehicleGoToHelicopter.h"
#include "vehicleAi/Task/TaskVehicleGoToPlane.h"
#include "vehicleAi/Task/TaskVehicleGoToSubmarine.h"
#include "vehicleAi/Task/TaskVehicleAttack.h"
#include "vehicleAi/Task/TaskVehicleBlock.h"
#include "vehicleAi/Task/TaskVehicleCircle.h"
#include "vehicleAi/Task/TaskVehicleCruise.h"
#include "vehicleAi/Task/TaskVehicleDeadDriver.h"
#include "vehicleAi/Task/TaskVehicleEscort.h"
#include "vehicleAi/Task/TaskVehicleFlying.h"
#include "vehicleAi/Task/TaskVehicleFollowRecording.h"
#include "vehicleAi/Task/TaskVehicleGoToLongRange.h"
#include "vehicleAi/Task/TaskVehicleHeliProtect.h"
#include "vehicleAi/Task/TaskVehiclePark.h"
#include "vehicleAi/Task/TaskVehiclePoliceBehaviour.h"
#include "vehicleAi/Task/TaskVehicleTempAction.h"
#include "vehicleAi/Task/TaskVehicleThreePointTurn.h"
#include "vehicleAi/task/TaskVehicleAnimation.h"
#include "vehicleAi/task/TaskVehiclePursue.h"
#include "vehicleAi/task/TaskVehicleLandPlane.h"
#include "vehicles/Metadata/VehicleEntryPointInfo.h"
#include "vehicles/Planes.h"
#include "Weapons/Info/WeaponInfoManager.h"

//Rage headers
#include "script/wrapper.h"

SCRIPT_OPTIMISATIONS()
AI_OPTIMISATIONS()
AI_VEHICLE_OPTIMISATIONS()

namespace task_commands {
  void UpdatePedRagdollBoundsForScriptActivation(CPed* pPed) {
    // only need to do this if we're in the viewport, not in ragdoll already and our capsule physics isn't active
    if (pPed && pPed->GetIsVisibleInSomeViewportThisFrame() && !pPed->GetUsingRagdoll()) {
      pPed->UpdateRagdollRootTransformFromAnimatedSkel();
      pPed->UpdateRagdollBoundsFromAnimatedSkel();
    }
  }

  void CommandTaskToggleDuck(int PedIndex, s32 iToggleType) {
    if (SCRIPT_VERIFY(CGameConfig::Get().AllowCrouchedMovement(), "TASK_TOGGLE_DUCK - Crouch is disabled")) {
      CTask* pTask = rage_new
      CTaskCrouchToggle(iToggleType);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_TOGGLE_DUCK, "TASK_TOGGLE_DUCK");
    }
  }

  void CommandTaskPause(int PedIndex, int Time) {
    CTask* pTask = rage_new
    CTaskPause(Time);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_PAUSE, "TASK_PAUSE");
  }

  void CommandTaskStandStill(int PedIndex, int StandStillTime) {
    if (SCRIPT_VERIFY((StandStillTime == -1) || (StandStillTime > 0), "TASK_STAND_STILL - Time must be -1 for infinite, otherwise > 0")) {
      const int numFramesToRun = 0; // no frame limit
      const bool bMakeMountStandStill = true;
      const bool bEnableTimeslicing = (StandStillTime == -1);
      CTask* pTask = rage_new
      CTaskDoNothing(StandStillTime, numFramesToRun, bMakeMountStandStill, bEnableTimeslicing);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_STAND_STILL, "TASK_STAND_STILL");
    }
  }

  void CommandTaskJump(int PedIndex, bool UNUSED_PARAM(UsePlayerLaunchForce), bool DoSuperJump, bool UseFullSuperJumpForce) {
    s32 nFlags = JF_DisableVault;

    if (DoSuperJump)
      nFlags |= JF_SuperJump | JF_BeastJump;

    if (UseFullSuperJumpForce)
      nFlags |= JF_AIUseFullForceBeastJump;

    CTask* pTask = rage_new
    CTaskJumpVault(nFlags);

    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_JUMP, "TASK_JUMP");
  }

  void CommandTaskCower(int PedIndex, int Time) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_COWER - Time must be -1 for infinite and > 0")) {
      CTask* pTask = rage_new
      CTaskCower(Time);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_COWER, "TASK_COWER");
    }
  }

  void CommandTaskHandsUp(int PedIndex, int Time, int PedToFaceIndex, int TimeToFacePed, int Flags) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_HANDS_UP - Time must be -1 for infinite and > 0")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedToFaceIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES_NO_DEAD_CHECK ^ CTheScripts::GUID_ASSERT_FLAG_ENTITY_EXISTS);
      CTask* pTask = rage_new
      CTaskHandsUp(Time, pPed, TimeToFacePed, Flags);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_HANDS_UP, "TASK_HANDS_UP");
    }
  }

  void CommandUpdateTaskHandsUpDuration(int PedIndex, int Time) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskHandsUp* pTaskHandsUp = static_cast<CTaskHandsUp*>(pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_HANDS_UP));
      if (SCRIPT_VERIFY(pTaskHandsUp, "UPDATE_TASK_HANDS_UP_DURATION - task is not running!")) {
        pTaskHandsUp->SetDuration(Time);
      }
    }
  }

  void CommandTaskDuck(int PedIndex, int Time) {
    if (SCRIPT_VERIFY(CGameConfig::Get().AllowCrouchedMovement(), "TASK_DUCK - Crouch is disabled")) {
      if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_DUCK - Time must be -1 for infinite and > 0")) {
        CTask* pTask;

        if (-1 == Time) {
          pTask = rage_new
          CTaskCrouch(0);
        } else {
          pTask = rage_new
          CTaskCrouch(Time);
        }
        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_DUCK, "TASK_DUCK");
      }
    }
  }

  void ScriptPassengerTask(int iPedID, CVehicle* pVehicle, int iTime, SeatRequestType requestType, int iSeat, int UNUSED_PARAM(EnterStatus), float fMoveBlendRatio, VehicleEnterExitFlags iScriptFlags, fwMvClipSetId overrideClipsetId, const char* commandName) {
    float fTime = (float) iTime;

    fTime /= 1000.0f;

    if (-1 == iTime) {
      fTime = 0.0f;
    } else if (iTime > 0) {
      iScriptFlags.BitSet().Set(CVehicleEnterExitFlags::WarpAfterTime);
    }

    // Convert the time into seconds
    CTask* pTask = rage_new
    CTaskEnterVehicle(pVehicle, requestType, iSeat, iScriptFlags, fTime, fMoveBlendRatio, NULL, overrideClipsetId);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_ENTER_VEHICLE, commandName);
  }

  void ScriptDriverTask(int iPedID, CVehicle* pVehicle, int iTime, int UNUSED_PARAM(EnterStatus), VehicleEnterExitFlags iScriptFlags, float fMoveBlendRatio, fwMvClipSetId overrideClipsetId, const char* commandName) {
    s32 seat = pVehicle->GetDriverSeat();

    float fTime = (float) iTime;

    if (-1 == iTime) {
      fTime = 0.0f;
    } else if (iTime >= 0) {
      iScriptFlags.BitSet().Set(CVehicleEnterExitFlags::WarpAfterTime);
    }

    // Convert the time into seconds
    fTime /= 1000.0f;

    CScriptPeds::GivePedScriptedTask(iPedID, rage_new
    CTaskEnterVehicle(pVehicle, SR_Specific, seat, iScriptFlags, fTime, fMoveBlendRatio, NULL, overrideClipsetId), SCRIPT_TASK_ENTER_VEHICLE, commandName);
  }

  void CommandTaskOpenVehicleDoor(int iPedID, int iVehicleID, int iTime, int iSeat, float fMoveBlendRatio) {
    SeatRequestType seatRequestType = SR_Specific;
    s32 iSeatRequest = iSeat + 1;
    if (iSeat == -2) {
      seatRequestType = SR_Any;
    }

    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_OPEN_VEHICLE_DOOR - Time must be -1 for infinite and > 0")) {
      CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (pVehicle) {
        VehicleEnterExitFlags vehicleFlags;
        vehicleFlags.BitSet().Set(CVehicleEnterExitFlags::JustOpenDoor);
        if (iSeatRequest != 0) {
          if (SCRIPT_VERIFY(iSeatRequest < pVehicle->GetSeatManager()->GetMaxSeats(), "TASK_OPEN_VEHICLE_DOOR - seat doesn't exist")) {
            ScriptPassengerTask(iPedID, pVehicle, iTime, seatRequestType, iSeatRequest, 0, fMoveBlendRatio, vehicleFlags, CLIP_SET_ID_INVALID, "TASK_OPEN_VEHICLE_DOOR");
          }
        } else {
          if (SCRIPT_VERIFY(!pVehicle->GetDriver(), "TASK_OPEN_VEHICLE_DOOR - Vehicle already has a driver")) {
            ScriptDriverTask(iPedID, pVehicle, iTime, 0, vehicleFlags, fMoveBlendRatio, CLIP_SET_ID_INVALID, "TASK_OPEN_VEHICLE_DOOR");
          }
        }
      }
    }
  }

  void CommandTaskEnterVehicle(int iPedID, int iVehicleID, int iTime, int iSeat, float fMoveBlendRatio, const int iScriptFlags, const char* szOverrideEntryClipsetName) {
    VehicleEnterExitFlags iFlags;
    CTaskVehicleFSM::SetScriptedVehicleEntryExitFlags(iFlags, iScriptFlags);

    SeatRequestType seatRequestType = SR_Specific;
    s32 iSeatRequest = iSeat + 1;
    if (iSeat == -2) {
      seatRequestType = SR_Any;
    }

    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_ENTER_VEHICLE - Time must be -1 for infinite and > 0")) {
      CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (pVehicle) {
        #if __ASSERT
                                                                                                                                const bool bIsTrailerLarge = MI_TRAILER_TRAILERLARGE.IsValid() && pVehicle->GetModelIndex() == MI_TRAILER_TRAILERLARGE;
				if (bIsTrailerLarge)
				{
					AI_LOG_WITH_ARGS("Ped with index %i is being placed into the trailerlarge from %s", iPedID, CTheScripts::GetCurrentScriptNameAndProgramCounter());
					AI_LOG_STACK_TRACE(8);
				}
        #endif // __ASSERT
        fwMvClipSetId overrideClipsetId = CLIP_SET_ID_INVALID;

        //look for an clip set with this name
        if (szOverrideEntryClipsetName) {
          if (fwClipSetManager::GetClipSet(fwMvClipSetId(szOverrideEntryClipsetName))) {
            overrideClipsetId.SetFromString(szOverrideEntryClipsetName);
            #if __ASSERT
            scriptAssertf(fwClipSetManager::IsStreamedIn_DEPRECATED(overrideClipsetId), "TASK_ENTER_VEHICLE - Clip set %s is not loaded yet:", szOverrideEntryClipsetName);
            #endif // __ASSERT
          }
          #if __ASSERT
                                                                                                                                  else
					{
						scriptAssertf(0, "Couldn't find clipset with name %s", szOverrideEntryClipsetName);
					}
          #endif // __ASSERT
        }

        if (iSeatRequest != 0) {
          if (SCRIPT_VERIFY(iSeatRequest < pVehicle->GetSeatManager()->GetMaxSeats(), "TASK_ENTER_VEHICLE - seat doesn't exist")) {
            ScriptPassengerTask(iPedID, pVehicle, iTime, seatRequestType, iSeatRequest, 0, fMoveBlendRatio, iFlags, overrideClipsetId, "TASK_ENTER_VEHICLE");
          }
        } else {
          // See if the vehicle has a driver already, if they are still running the enter vehicle task then they must be shuffling
          // so discount them
          CPed* pDriver = pVehicle->GetDriver();
          if (pDriver && pDriver->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ENTER_VEHICLE)) {
            pDriver = NULL;
          }

          //driver seat
          if (SCRIPT_VERIFY(!pDriver || (iFlags.BitSet().IsSet(CVehicleEnterExitFlags::JackIfOccupied)), "TASK_ENTER_VEHICLE - Vehicle already has a driver")) {
            ScriptDriverTask(iPedID, pVehicle, iTime, 0, iFlags, fMoveBlendRatio, overrideClipsetId, "TASK_ENTER_VEHICLE");
          }
        }
      }
    }

  }

  void CommandTaskLeaveVehicle(int iPedID, int iVehicleID, s32 iScriptFlags) {
    CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pVehicle) {
      VehicleEnterExitFlags vehicleFlags;
      CTaskVehicleFSM::SetScriptedVehicleEntryExitFlags(vehicleFlags, iScriptFlags);
      vehicleFlags.BitSet().Set(CVehicleEnterExitFlags::DontJackAnyone);
      CTask* pTask = rage_new
      CTaskExitVehicle(pVehicle, vehicleFlags);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_LEAVE_VEHICLE, "TASK_LEAVE_VEHICLE");
    }
  }

  void CommandTaskVehicleDriveToCoord_Intern(int iPedID, int iVehicleID, const Vector3& VecCoors_, float CruiseSpeed, int iMode, int UNUSED_PARAM(VehicleModelHashKey), int iDrivingFlags, float TargetRadius, float StraightLineDist) {
    Vector3 VecCoors = VecCoors_;

    TUNE_GROUP_BOOL(RACING_AI, RACING_actuallyGoFast, false);
    TUNE_GROUP_FLOAT(RACING_AI, RACING_actuallyGoFastSpeed, 50.0f, 0.0, 100.0f, 0.1f);
    if (RACING_actuallyGoFast) {
      CruiseSpeed = RACING_actuallyGoFastSpeed;
    }

    CTask* pTask = NULL;
    CVehicle* pVehicle = NULL;

    pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!pVehicle) {
      return;
    }

    if (VecCoors.z <= INVALID_MINIMUM_WORLD_Z) {
      VecCoors.z = WorldProbe::FindGroundZForCoord(TOP_SURFACE, VecCoors.x, VecCoors.y);
    }

    if (StraightLineDist < 0.0f) {
      StraightLineDist = sVehicleMissionParams::DEFAULT_SWITCH_TO_STRAIGHT_LINE_DIST;
    }

    // The old driving styles are still present in the script headers, until we remove them this will
    // At least make drivingstyle reversing work.
    if (iMode == 2) {
      iDrivingFlags |= DF_DriveInReverse;
    }

    aiTask* pVehicleTask = CVehicleIntelligence::GetGotoTaskForVehicle(pVehicle, NULL, &VecCoors, iDrivingFlags, TargetRadius, StraightLineDist, CruiseSpeed);
    pTask = rage_new
    CTaskControlVehicle(pVehicle, pVehicleTask);

    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_DRIVE_TO_COORD, "TASK_VEHICLE_DRIVE_TO_COORD");
  }

  void CommandTaskVehicleDriveToCoord(int iPedID, int iVehicleID, const scrVector& scrVecCoors, float CruiseSpeed, int iMode, int VehicleModelHashKey, int iDrivingFlags, float TargetRadius, float StraightLineDist) {
    Vector3 VecCoors = Vector3(scrVecCoors);

    CommandTaskVehicleDriveToCoord_Intern(iPedID, iVehicleID, VecCoors, CruiseSpeed, iMode, VehicleModelHashKey, iDrivingFlags, TargetRadius, StraightLineDist);
  }

  void CommandTaskVehicleDriveToCoordLongRange(int iPedID, int iVehicleID, const scrVector& scrVecCoors, float CruiseSpeed, int iDrivingFlags, float TargetRadius) {
    Vector3 VecCoors = Vector3(scrVecCoors);

    CTask* pTask = NULL;
    CVehicle* pVehicle = NULL;

    pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!pVehicle) {
      return;
    }

    if (VecCoors.z <= INVALID_MINIMUM_WORLD_Z) {
      VecCoors.z = WorldProbe::FindGroundZForCoord(TOP_SURFACE, VecCoors.x, VecCoors.y);
    }

    //aiTask *pVehicleTask = CVehicleIntelligence::GetGotoTaskForVehicle(pVehicle, NULL, &VecCoors, iDrivingFlags, TargetRadius, StraightLineDist, CruiseSpeed);
    sVehicleMissionParams params;
    params.SetTargetPosition(VecCoors);
    params.m_iDrivingFlags = iDrivingFlags;
    params.m_fTargetArriveDist = TargetRadius;
    params.m_fCruiseSpeed = CruiseSpeed;

    CTask* pVehicleTask = rage_new
    CTaskVehicleGotoLongRange(params);
    pTask = rage_new
    CTaskControlVehicle(pVehicle, pVehicleTask);

    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE, "TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE");
  }

  #if ENABLE_HORSE

                                                                                                                          void CommandTaskMountAnimal( int iPedID, int iMountID, int iTime, int iSeat, float fMoveBlendRatio, const int iFlags )
	{
		s32 iSeatRequest = iSeat+1;

		if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0) ,"TASK_MOUNT_ANIMAL - Time must be -1 for infinite and > 0" ))
		{
			// if this task is being added to a sequence, don't do a clone check on the mount
			unsigned assertFlags = (iPedID == NULL_IN_SCRIPTING_LANGUAGE) ? CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES : CTheScripts::GUID_ASSERT_FLAGS_ALL;

			CPed *pMount = CTheScripts::GetEntityToModifyFromGUID<CPed>(iMountID, assertFlags);
			if (pMount)
			{
				if (iSeatRequest != 0)
				{
					scriptAssertf(false, "Non-driver's on mounts not yet supported by TASK_MOUNT_ANIMAL");
				}
				else
				{
					VehicleEnterExitFlags vehicleFlags;
					CTaskVehicleFSM::SetScriptedVehicleEntryExitFlags(vehicleFlags, iFlags);

					//driver seat
					if (SCRIPT_VERIFY(!pMount->GetSeatManager()->GetDriver() || vehicleFlags.BitSet().IsSet(CVehicleEnterExitFlags::JackIfOccupied), "TASK_MOUNT_ANIMAL - Mount already has a driver"))
					{
						s32 seat = pMount->GetPedModelInfo()->GetModelSeatInfo()->GetDriverSeat();

						float fTime = (float)iTime;
						if( -1==iTime )
						{
							fTime = 0.0f;
						}
						else if( iTime >= 0 )
						{
							vehicleFlags.BitSet().Set(CVehicleEnterExitFlags::WarpAfterTime);
						}
						// Convert the time into seconds
						fTime/=1000.0f;

						CScriptPeds::GivePedScriptedTask( iPedID, rage_new CTaskMountAnimal(pMount, SR_Specific, seat, vehicleFlags, fTime, fMoveBlendRatio), SCRIPT_TASK_MOUNT_ANIMAL, "TASK_MOUNT_ANIMAL");
					}
				}
			}
		}
	}

	void CommandTaskDismountAnimal( int iPedID, s32 iScriptFlags )
	{
		CPed *pMount = NULL;
		if(iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
			CPed *pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
			if (pPed)
			{
				pMount = pPed->GetMyMount();
				if (!pMount)
				{
					return;
				}
			}
		}

		VehicleEnterExitFlags vehicleFlags;
		CTaskVehicleFSM::SetScriptedVehicleEntryExitFlags(vehicleFlags, iScriptFlags);
		vehicleFlags.BitSet().Set(CVehicleEnterExitFlags::DontJackAnyone);
		CTask* pTask=rage_new CTaskDismountAnimal(vehicleFlags);
		CScriptPeds::GivePedScriptedTask( iPedID, pTask, SCRIPT_TASK_DISMOUNT_ANIMAL, "TASK_DISMOUNT_ANIMAL");
	}

  #else

  void CommandTaskMountAnimal(int UNUSED_PARAM(iPedID), int UNUSED_PARAM(iMountID), int UNUSED_PARAM(iTime), int UNUSED_PARAM(iSeat), float UNUSED_PARAM(fMoveBlendRatio), const int UNUSED_PARAM(iFlags)) {
  }

  void CommandTaskDismountAnimal(int UNUSED_PARAM(iPedID), s32 UNUSED_PARAM(iScriptFlags)) {
  }

  #endif

  void CommandTaskVehicleDriveWander(int iPedID, int iVehicleID, float fSpeed, int iDrivingFlags) {
    CVehicle* pVehicle = NULL;
    if (iVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!SCRIPT_VERIFY(pVehicle, "TASK_VEHICLE_DRIVE_WANDER needs a valid vehicle")) {
      return;
    }

    // cap cruise speed to the maximum
    float fCruiseSpeed = rage::Min(fSpeed, CTaskVehicleMissionBase::MAX_CRUISE_SPEED);

    CTask* pTask = rage_new
    CTaskCarDriveWander(pVehicle, iDrivingFlags, fCruiseSpeed);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_DRIVE_WANDER, "TASK_VEHICLE_DRIVE_WANDER");
  }

  void CommandTaskGoStraightToCoord(int PedIndex, const scrVector& scrVecCoors, float MoveBlendRatio, int Time, const float fTargetHeading, const float fInTargetRadius) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_GO_STRAIGHT_TO_COORD - Time must be -1 for infinite and > 0")) {
      CTask* pTask;

      Vector3 vTarget(scrVecCoors);

      const float fTargetRadius = fInTargetRadius;//CTaskMoveGoToPointAndStandStill::ms_fTargetRadius;
      const float fSlowDownDistance = CTaskMoveGoToPointAndStandStill::ms_fSlowDownDistance;
      float fMainTaskTimer = 0.0f;
      if (-1 == Time) {
        pTask = rage_new
        CTaskMoveGoToPointAndStandStill(MoveBlendRatio, vTarget, fTargetRadius, fSlowDownDistance, false, true, fTargetHeading < DEFAULT_NAVMESH_FINAL_HEADING ? DtoR * fTargetHeading : fTargetHeading);
      } else {
        fMainTaskTimer = (float) Time / 1000.0f;
        if (fMainTaskTimer > 0.0f)
          fMainTaskTimer += 2.0f;

        int iTime = Max(CTaskComplexGoToPointAndStandStillTimed::ms_iTime, Time);
        pTask = rage_new
        CTaskComplexGoToPointAndStandStillTimed(MoveBlendRatio, vTarget, fTargetRadius, fSlowDownDistance, iTime, true, fTargetHeading < DEFAULT_NAVMESH_FINAL_HEADING ? DtoR * fTargetHeading : fTargetHeading);
      }

      pTask = rage_new
      CTaskComplexControlMovement(pTask, NULL, CTaskComplexControlMovement::TerminateOnMovementTask, fMainTaskTimer);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_STRAIGHT_TO_COORD, "TASK_GO_STRAIGHT_TO_COORD");
    }
  }

  void CommandTaskGoStraightToCoordRelativeToEntity(int PedIndex, int EntityIndex, const scrVector& scrVecCoors, float MoveBlendRatio, int Time) {
    Vector3 vLocalTarget(scrVecCoors);
    const float fTargetRadius = CTaskMoveGoToPointAndStandStill::ms_fTargetRadius;

    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY - Time must be -1 for infinite and > 0")) {
      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(EntityIndex);
      if (pEntity) {
        CTaskMoveGoToPointRelativeToEntityAndStandStill* pMoveTask = rage_new
        CTaskMoveGoToPointRelativeToEntityAndStandStill(
          pEntity,
          MoveBlendRatio,
          vLocalTarget,
          fTargetRadius,
          Time
        );

        CTask* pTask = rage_new
        CTaskComplexControlMovement(pMoveTask);
        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY, "TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY");
      }
    }
  }

  void CommandTaskAchieveHeading(int PedIndex, float fHeadingDegrees, int iTime) {
    float fTime;
    if (iTime < 0) {
      fTime = -1.0f;
    } else if (iTime == 0) {
      fTime = 0.0f;
    } else {
      fTime = ((float) iTime) / 1000.0f;
    }
    const float fHeadingRadians = (DtoR * fHeadingDegrees);
    CTask* pTask = rage_new
    CTaskMoveAchieveHeading(fHeadingRadians, CTaskMoveAchieveHeading::ms_fHeadingChangeRateFrac, CTaskMoveAchieveHeading::ms_fHeadingTolerance, fTime);
    pTask = rage_new
    CTaskComplexControlMovement(pTask);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_ACHIEVE_HEADING, "TASK_ACHIEVE_HEADING");
  }

  void CommandFlushRoute() {
    CTaskMoveFollowPointRoute::ms_pointRoute.Clear();
  }

  void CommandExtendRoute(const scrVector& scrVecCoors) {
    Vector3 vTarget(scrVecCoors);
    if (CTaskMoveFollowPointRoute::ms_pointRoute.Add(vTarget) == false) {
      scriptAssertf(0, "%s:EXTEND_ROUTE - too many points in this route ", CTheScripts::GetCurrentScriptNameAndProgramCounter());
    }
  }

  void CommandTaskFollowToOffsetOfEntity(int PedIndex, int EntityIndex, const scrVector& scrVecOffset, float MoveBlendRatio, int Time, float Radius, bool RelativeOffset) {
    Vector3 Offset(scrVecOffset);
    const CPhysical* pEntity = CTheScripts::GetEntityToQueryFromGUID<CPhysical>(EntityIndex);

    if (pEntity) {
      if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_FOLLOW_TO_OFFSET_OF_PED - Time must be -1 for infinite and > 0")) {
        CTask* pTask = rage_new
        CTaskComplexControlMovement(rage_new
        CTaskMoveFollowEntityOffset(pEntity, MoveBlendRatio, Radius, Offset, Time, RelativeOffset));
        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_FOLLOW_TO_OFFSET_OF_ENTITY, "TASK_FOLLOW_TO_OFFSET_OF_ENTITY");
      }
    }
  }

  void CommandTaskFollowToOffsetOfPickup(int PedIndex, int PickupID, const scrVector& scrVecOffset, float MoveBlendRatio, int Time, float Radius, bool RelativeOffset) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_FOLLOW_TO_OFFSET_OF_PICKUP - Time must be -1 for infinite and > 0")) {
      if (scriptVerify(CTheScripts::GetCurrentGtaScriptHandler())) {
        scriptHandlerObject* pScriptHandlerObject = CTheScripts::GetCurrentGtaScriptHandler()->GetScriptObject(PickupID);

        if (scriptVerifyf(pScriptHandlerObject, "%s: TASK_FOLLOW_TO_OFFSET_OF_PICKUP - failed to find script handler object for pickup ID", CTheScripts::GetCurrentScriptNameAndProgramCounter())) {
          if (scriptVerifyf(static_cast<CGameScriptHandlerObject*>(pScriptHandlerObject)->GetType() == SCRIPT_HANDLER_OBJECT_TYPE_PICKUP, "%s: TASK_FOLLOW_TO_OFFSET_OF_PICKUP - This is not a pickup id", CTheScripts::GetCurrentScriptNameAndProgramCounter())) {
            CPickupPlacement* pPlacement = static_cast<CPickupPlacement*>(pScriptHandlerObject);

            if (SCRIPT_VERIFY(pPlacement, "TASK_FOLLOW_TO_OFFSET_OF_PICKUP - Pickup does not exist")) {
              if (SCRIPT_VERIFY(!pPlacement->GetIsCollected(), "TASK_FOLLOW_TO_OFFSET_OF_PICKUP - pickup has already been collected")) {
                if (SCRIPT_VERIFY(!pPlacement->GetHasPickupBeenDestroyed(), "TASK_FOLLOW_TO_OFFSET_OF_PICKUP - pickup has been destroyed")) {
                  const CPickup* pPickup = pPlacement->GetPickup();
                  if (SCRIPT_VERIFY(pPickup, "TASK_FOLLOW_TO_OFFSET_OF_PICKUP - pickup object for this pickup placement is NULL")) {
                    Vector3 Offset(scrVecOffset);
                    CTask* pTask = rage_new
                    CTaskComplexControlMovement(rage_new
                    CTaskMoveFollowEntityOffset(pPickup, MoveBlendRatio, Radius, Offset, Time, RelativeOffset));
                    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_FOLLOW_TO_OFFSET_OF_PICKUP, "TASK_FOLLOW_TO_OFFSET_OF_PICKUP");
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void CommandTaskFollowPointRoute(int PedIndex, float MoveBlendRatio, int Mode) {
    if (SCRIPT_VERIFY(((CTaskMoveFollowPointRoute::TICKET_SINGLE == Mode) ||
                       (CTaskMoveFollowPointRoute::TICKET_RETURN == Mode) ||
                       (CTaskMoveFollowPointRoute::TICKET_SEASON == Mode) ||
                       (CTaskMoveFollowPointRoute::TICKET_LOOP == Mode)),
                      "TASK_FOLLOW_POINT_ROUTE - invalid mode")) {
      CTask* pTask = rage_new
      CTaskMoveFollowPointRoute(MoveBlendRatio, CTaskMoveFollowPointRoute::ms_pointRoute, Mode);
      pTask = rage_new
      CTaskComplexControlMovement(pTask);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_FOLLOW_POINT_ROUTE, "TASK_FOLLOW_POINT_ROUTE");
    }
  }

  // Please keep this in sync with the enumeration in "commands_task.sch"
  enum EGOTO_ENTITY_SCRIPT_FLAGS {
    EGOTO_ENTITY_NEVER_SLOW_FOR_PATH_LENGTH = 0x01,
  };

  void CommandTaskGoToEntity(int PedIndex, int EntityIndex, int Time, float SeekRadius, float MoveBlendRatio, float fSlowDownDistance, int iFlags) {
    const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(EntityIndex);

    if (pEntity) {
      if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_GOTO_ENTITY - Time must be -1 for infinite and > 0")) {
        if (NetworkInterface::IsGameInProgress() && (!pEntity->GetIsDynamic() || !((CDynamicEntity*) pEntity)->GetNetworkObject())) {
          scriptAssertf(0, "%s:TASK_GOTO_ENTITY; the entity must be networked or this ped will not be able to migrate properly", CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }

        CTask* pMoveTask = rage_new
        TTaskMoveSeekEntityStandard(pEntity, Time, TTaskMoveSeekEntityStandard::ms_iPeriod, SeekRadius);
        ((TTaskMoveSeekEntityStandard*) pMoveTask)->SetMoveBlendRatio(MoveBlendRatio);
        // scripted seek entity, so we use larger search extents
        ((TTaskMoveSeekEntityStandard*) pMoveTask)->SetUseLargerSearchExtents(true);
        ((TTaskMoveSeekEntityStandard*) pMoveTask)->SetUseAdaptiveUpdateFreq(true);
        ((TTaskMoveSeekEntityStandard*) pMoveTask)->SetSlowDownDistance(fSlowDownDistance);

        if ((iFlags & EGOTO_ENTITY_NEVER_SLOW_FOR_PATH_LENGTH) != 0) {
          ((TTaskMoveSeekEntityStandard*) pMoveTask)->SetNeverSlowDownForPathLength(true);
        }

        CTask* pTask = rage_new
        CTaskComplexControlMovement(pMoveTask);

        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_TO_ENTITY, "TASK_GOTO_ENTITY");
      }
    }
  }

  void CommandTaskSmartFleeCoord(int PedIndex, const scrVector& scrVecCoors, float SafeDistance, int Time, bool UNUSED_PARAM(bPreferPavements), bool bQuitIfOutOfRange) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_SMART_FLEE_POINT - Time must be -1 for infinite and > 0")) {
      if (SCRIPT_VERIFY(SafeDistance >= 0.0f, "TASK_SMART_FLEE_POINT - SafeDistance must be >= 0.0")) {
        Vector3 vTarget(scrVecCoors);
        CTaskSmartFlee* pTask = rage_new
        CTaskSmartFlee(CAITarget(vTarget), SafeDistance, Time);
        // scripted flee task, so we use larger search extents
        pTask->SetUseLargerSearchExtents(true);
        pTask->SetQuitIfOutOfRange(bQuitIfOutOfRange);

        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SMART_FLEE_POINT, "TASK_SMART_FLEE_POINT");
      }
    }
  }

  void CommandTaskSmartFleePed(int PedIndex, int OtherPedIndex, float SafeDistance, int Time, bool UNUSED_PARAM(bPreferPavements), bool UNUSED_PARAM(bUpdateToNearestHatedPed)) {
    if (SCRIPT_VERIFY((Time == -1) || (Time > 0), "TASK_SMART_FLEE_PED - Time must be -1 for infinite and > 0")) {
      if (SCRIPT_VERIFY(SafeDistance >= 0.0f, "TASK_SMART_FLEE_PED - SafeDistance must be >= 0.0")) {
        const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(OtherPedIndex);
        if (pOtherPed) {
          CTaskSmartFlee* pTask = rage_new
          CTaskSmartFlee(CAITarget(pOtherPed),              // Target to flee from
                         SafeDistance,                  // Distance to stop at
                         Time,                      // Time to flee for
                         CTaskSmartFlee::ms_uiEntityPosCheckPeriod,
                         0.0f, /*CTaskSmartFlee::ms_fEntityPosChangeThreshold*/
                         false);

          // scripted flee task, so we use larger search extents
          pTask->SetUseLargerSearchExtents(true);
          pTask->SetConsiderRunStartForPathLookAhead(true);

          CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SMART_FLEE_PED, "TASK_SMART_FLEE_PED");
        }
      }
    }
  }

  void CommandTaskReactAndFleeCoord(int PedIndex, const scrVector& scrVecCoors) {
    Vector3 vTarget(scrVecCoors);
    CTaskReactAndFlee* pTask = rage_new
    CTaskReactAndFlee(CAITarget(vTarget), CTaskReactAndFlee::BA_Gunfire);

    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_REACT_AND_FLEE_COORD, "TASK_REACT_AND_FLEE_COORD");
  }

  void CommandTaskReactAndFleePed(int PedIndex, int OtherPedIndex) {
    const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(OtherPedIndex);
    if (pOtherPed) {
      CTaskReactAndFlee* pTask = rage_new
      CTaskReactAndFlee(CAITarget(pOtherPed), CTaskReactAndFlee::BA_Gunfire);

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_REACT_AND_FLEE_PED, "TASK_REACT_AND_FLEE_PED");
    }
  }

  CEventShocking* GetShockingEventFromIDInternal(int EventIndex) {
    CEventGroupGlobal& global = *GetEventGlobalGroup();
    int num = global.GetNumEvents();
    for (int j = 0; j < num; j++) {
      fwEvent* ev = global.GetEventByIndex(j);
      if (!ev || !static_cast<CEvent*>(ev)->IsShockingEvent()) {
        continue;
      }

      CEventShocking* pShocking = static_cast<CEventShocking*>(ev);

      // Check if we've found the event we're looking for.
      // Note: GetCreatedByScript() is here in case a script-created event timed out by itself,
      // and then a code-created one reused the same ID (which would require a wraparound, so it's
      // unlikely anyway). Since there are currently no commands to give scripts the ID numbers
      // of code-created shocking events, the only valid use of this command is for script-created
      // events.
      if (EventIndex == (int) pShocking->GetId() && pShocking->GetCreatedByScript()) {
        return pShocking;
      }
    }
    return NULL;
  }

  void CommandTaskShockingEventBackAway(int PedIndex, int EventIndex) {
    CEventShocking* pEvent = GetShockingEventFromIDInternal(EventIndex);

    if (SCRIPT_VERIFY(pEvent, "Invalid event specified to react to!")) {
      const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
      if (SCRIPT_VERIFY(pPed, "Invalid PedIndex!")) {
        CTask* pTask = rage_new
        CTaskShockingEventBackAway(pEvent, CTaskShockingEventBackAway::GetDefaultBackAwayPositionForPed(*pPed));

        CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SHOCKING_EVENT_BACK_AWAY, "TASK_SHOCKING_EVENT_BACK_AWAY");
      }
    }
  }

  void CommandTaskShockingEventHurryAway(int PedIndex, int EventIndex) {
    CEventShocking* pEvent = GetShockingEventFromIDInternal(EventIndex);

    if (SCRIPT_VERIFY(pEvent, "Invalid event specified to react to!")) {
      CTask* pTask = rage_new
      CTaskShockingEventHurryAway(pEvent);

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SHOCKING_EVENT_HURRY_AWAY, "TASK_SHOCKING_EVENT_HURRY_AWAY");
    }
  }

  void CommandTaskShockingEventReact(int PedIndex, int EventIndex) {
    CEventShocking* pEvent = GetShockingEventFromIDInternal(EventIndex);

    if (SCRIPT_VERIFY(pEvent, "Invalid event specified to react to!")) {
      CTask* pTask = rage_new
      CTaskShockingEventReact(pEvent);

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SHOCKING_EVENT_REACT, "TASK_SHOCKING_EVENT_REACT");
    }
  }

  void CommandTaskWanderInArea(int PedIndex, const scrVector& scvCenter, float fRadius, float fMinWaitTime, float fMaxWaitTime) {
    if (SCRIPT_VERIFY(fMinWaitTime <= fMaxWaitTime, "MinWaitTime was greater than MaxWaitTime")) {
      Vec3V vCenter(scvCenter);
      CTask* pTask = rage_new
      CTaskWanderInArea(MOVEBLENDRATIO_WALK, fRadius, VEC3V_TO_VECTOR3(vCenter), fMinWaitTime, fMaxWaitTime);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_WANDER_IN_AREA, "TASK_WANDER_IN_AREA");
    }
  }

  void CommandTaskWanderStandard(int PedIndex, float fHeading, int iFlags) {
    float fWanderHeading = (IsClose(fHeading, DEFAULT_NAVMESH_FINAL_HEADING, 0.1f) ? fwRandom::GetRandomNumberInRange(0.0f, TWO_PI) : fHeading * DtoR);
    CTaskWander* pTask = rage_new
    CTaskWander(MOVEBLENDRATIO_WALK, fWanderHeading);

    if ((iFlags & EWDR_KEEP_MOVING_WHILST_WAITING_FOR_PATH) != 0) {
      pTask->KeepMovingWhilstWaitingForFirstPath(NULL);
    }

    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_WANDER_STANDARD, "TASK_WANDER_STANDARD");
  }

  void CommandTaskWanderSpecific(int PedIndex, const char* CondtionalAnimGroupStr, const char* ConditonalAnimStr, float fHeading) {
    // Find the conditionalAnimsGroup
    const CConditionalAnimsGroup* pConditionalAnimGroup = CONDITIONALANIMSMGR.GetConditionalAnimsGroup(atHashString(CondtionalAnimGroupStr));
    if (scriptVerifyf(pConditionalAnimGroup, "TASK_WANDER_SPECIFIC called with invalid ConditionalAnimGroup name, (%s)", CondtionalAnimGroupStr)) {
      // Search for the specified conditionalAnim
      atHashString conditionalAnimHash(ConditonalAnimStr);
      for (int scan = 0; scan < pConditionalAnimGroup->GetNumAnims(); ++scan) {
        const CConditionalAnims* pAnims = pConditionalAnimGroup->GetAnims(scan);
        if (pAnims && pAnims->GetNameHash() == conditionalAnimHash) {
          float fWanderHeading = (IsClose(fHeading, DEFAULT_NAVMESH_FINAL_HEADING, 0.1f) ? fwRandom::GetRandomNumberInRange(0.0f, TWO_PI) : fHeading * DtoR);
          CTask* pTask = rage_new
          CTaskWander(MOVEBLENDRATIO_WALK, fWanderHeading, pConditionalAnimGroup, scan);
          CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_WANDER_SPECIFIC, "TASK_WANDER_SPECIFIC");
          return;
        }
      }
      // If you get here, we failed to find the specified conditional anim.
      scriptAssertf(0, "TASK_WANDER_SPECIFIC could not find ConditionalAnim name (%s) in ConditionalAnimGroup (%s)", ConditonalAnimStr, CondtionalAnimGroupStr);
    }
  }

  void CommandTaskCombat(int iPedID, int iOtherPedID, const int iCombatFlags, const int iThreatResponseFlags) {
    // NULL peds are valid since this task can run in a sequence
    const CPed* pPed = NULL;
    if (iPedID != NULL_IN_SCRIPTING_LANGUAGE)
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);

    const CPed* pSecondPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedID);

    if (pSecondPed) {
      SCRIPT_ASSERT((!pPed || !pPed->GetPedIntelligence()->IsFriendlyWith(*pSecondPed)), "TASK_COMBAT_PED - Ped told to attack friendly ped!");
      SCRIPT_ASSERT(!pSecondPed->GetPedResetFlag(CPED_RESET_FLAG_CannotBeTargeted), "TASK_COMBAT_PED - Ped told to attack ped with flag PRF_CannotBeTargetedByAI!");

      CTaskThreatResponse* pTask = rage_new
      CTaskThreatResponse(pSecondPed);
      pTask->GetConfigFlags().SetAllFlags((u8) iThreatResponseFlags);
      pTask->GetConfigFlagsForCombat().SetAllFlags(iCombatFlags);

      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_COMBAT, "TASK_COMBAT_PED");
    }
  }

  void CommandTaskCombatHatedTargetsInArea(int PedIndex, const scrVector& scvCentre, float Radius, const int iCombatFlags) {
    if (NetworkInterface::IsGameInProgress()) {
      SCRIPT_ASSERT(Radius < CClonedCombatClosestTargetInAreaInfo::MAX_AREA, "Radius is greater than max allowed (300.0m)");
      Radius = rage::Clamp(Radius, 0.0f, CClonedCombatClosestTargetInAreaInfo::MAX_AREA);
    }

    Vector3 vCentre = scvCentre;
    CTask* pTask = rage_new
    CTaskCombatClosestTargetInArea(vCentre, Radius, false, CTaskCombatClosestTargetInArea::INVALID_OVERALL_TIME, iCombatFlags);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_COMBAT_HATED_TARGETS_IN_AREA, "TASK_COMBAT_HATED_PED_IN_AREA");
  }

  void CommandTaskCombatHatedTargetsAroundPed(int PedIndex, float Radius, const int iCombatFlags) {
    if (NetworkInterface::IsGameInProgress()) {
      SCRIPT_ASSERT(Radius < CClonedCombatClosestTargetInAreaInfo::MAX_AREA, "Radius is greater than max allowed (300.0m)");
      Radius = rage::Clamp(Radius, 0.0f, CClonedCombatClosestTargetInAreaInfo::MAX_AREA);
    }

    CTask* pTask = rage_new
    CTaskCombatClosestTargetInArea(Vector3(0.0f, 0.0f, 0.0f), Radius, true, CTaskCombatClosestTargetInArea::INVALID_OVERALL_TIME, iCombatFlags);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_COMBAT_HATED_TARGETS_AROUND_PED, "TASK_COMBAT_HATED_TARGETS_AROUND_PED");
  }

  void CommandTaskCombatHatedTargetsAroundPedTimed(int PedIndex, float Radius, int iTime, const int iCombatFlags) {
    if (NetworkInterface::IsGameInProgress()) {
      SCRIPT_ASSERT(Radius < CClonedCombatClosestTargetInAreaInfo::MAX_AREA, "Radius is greater than max allowed (300.0m)");
      Radius = rage::Clamp(Radius, 0.0f, CClonedCombatClosestTargetInAreaInfo::MAX_AREA);
    }

    float fTime = ((float) iTime) / 1000.0f;

    CTask* pTask = rage_new
    CTaskCombatClosestTargetInArea(Vector3(0.0f, 0.0f, 0.0f), Radius, true, fTime, iCombatFlags);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED, "TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED");
  }

  void CommandTaskCombatTimed(int PedIndex, int iOtherPedID, s32 iTime, const int iFlags) {
    CPed* pSecondPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iOtherPedID);
    if (pSecondPed) {
      if (NULL_IN_SCRIPTING_LANGUAGE != PedIndex) {
        const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
        if (pPed) {
          if (!SCRIPT_VERIFY(!pPed->GetPedIntelligence()->IsFriendlyWith(*pSecondPed), "TASK_COMBAT_TIMED - Ped told to attack friendly ped!"))
            return;
        }
      }

      float fTime = ((float) iTime) / 1000.0f;
      CTaskThreatResponse* pTask = rage_new
      CTaskThreatResponse(pSecondPed, fTime);
      pTask->GetConfigFlags().SetFlag(CTaskThreatResponse::CF_CanFightArmedPedsWhenNotArmed);
      pTask->GetConfigFlagsForCombat().SetAllFlags(iFlags);

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_COMBAT_TIMED, "TASK_COMBAT_TIMED");
    }
  }

  void CommandTaskThrowProjectile(int PedIndex, const scrVector& scrVecTarget, int IgnoreCollisionEntityIndex, bool CreateInvincibleProjectile) {
    CEntity* pIgnoreCollisionEntity = NULL;

    if (IgnoreCollisionEntityIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      pIgnoreCollisionEntity = CTheScripts::GetEntityToModifyFromGUID<CEntity>(IgnoreCollisionEntityIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES_NO_DEAD_CHECK);

      #if __BANK
                                                                                                                              if (!pIgnoreCollisionEntity)
			{
				scriptAssertf(0,"TASK_THROW_PROJECTILE - IgnoreCollisionEntityIndex is not null (%i), but could not find the entity in the world!", IgnoreCollisionEntityIndex);
			}
      #endif
    }

    // Create vectors needed for getting the start position, trajectory, velocity.
    CTaskAimAndThrowProjectile* pTask = rage_new
    CTaskAimAndThrowProjectile(CWeaponTarget(scrVecTarget), pIgnoreCollisionEntity, CreateInvincibleProjectile);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_THROW_PROJECTILE, "TASK_THROW_PROJECTILE");
  }

  //*************************************************************************************************************************



  CTaskMoveFollowNavMesh* CreateNavigateTask(const int iPedIndex, const Vector3& vTarget, const float fMoveBlendRatio, int iTime, const float fTargetRadius, const float fTargetHeading = DEFAULT_NAVMESH_FINAL_HEADING) {
    //*************************************************************
    // Assert that scripters aren't trying to do "crazy shit"(tm)

    if (iPedIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);

      if (!pPed)
        return NULL;
    }

    //********************************
    // Sanity-check for comedy values

    ASSERT_ONLY(
    static const float fStupidlyLargeValue = 10000.0f);
    scriptAssertf(rage::FPIsFinite(vTarget.x) && rage::FPIsFinite(vTarget.y) && rage::FPIsFinite(vTarget.z), "script \'%s\'\nNAVIGATE_TO_COORD - Ped %i's target (%.2f, %.2f, %.2f) is duff.", CTheScripts::GetCurrentScriptNameAndProgramCounter(), iPedIndex, vTarget.x, vTarget.y, vTarget.z);
    scriptAssertf((vTarget.x > -fStupidlyLargeValue && vTarget.y > -fStupidlyLargeValue && vTarget.z > -fStupidlyLargeValue && vTarget.x < fStupidlyLargeValue && vTarget.y < fStupidlyLargeValue && vTarget.z < fStupidlyLargeValue), "script \'%s\'\nNAVIGATE_TO_COORD - Ped %i's target (%.2f, %.2f, %.2f) is outside of the world.", CTheScripts::GetCurrentScriptNameAndProgramCounter(), iPedIndex, vTarget.x, vTarget.y, vTarget.z);

    //***************************************************************
    // Translate iTime into a value meaningful to the navmesh task

    //-1 is assumed as to be infinite -2, will assert legacy used to be infinite

    //*********************************************************
    // Create the CTaskMoveFollowNavMesh task
    // Set up the task according to the passed-in parameters

    CTaskMoveFollowNavMesh* pTaskFollowNavMesh = rage_new
    CTaskMoveFollowNavMesh(
      fMoveBlendRatio,
      vTarget,
      fTargetRadius,
      CTaskMoveFollowNavMesh::ms_fSlowDownDistance,
      iTime,
      true,
      false,
      NULL,
      fTargetRadius);

    //***********************************************
    // This is always set for scripted navmesh tasks

    pTaskFollowNavMesh->SetIsScriptedRoute(true);

    pTaskFollowNavMesh->SetTargetStopHeading(fTargetHeading);

    return pTaskFollowNavMesh;
  }

  void CommandTaskFollowNavMeshToCoord(const int iPedIndex, const scrVector& vScrTarget, const float fMoveBlendRatio, const int iTime, const float fTargetRadius, const int iFlags, const float fTargetHeading) {
    Assertf(fMoveBlendRatio >= MOVEBLENDRATIO_STILL && fMoveBlendRatio <= MOVEBLENDRATIO_SPRINT, "Invalid 'fMoveBlendRatio' specified in TASK_FOLLOW_NAVMESH_TO_COORD.  Must be in range (%.1f to %.1f) inclusive.  You've passed in %.1f", MOVEBLENDRATIO_STILL, MOVEBLENDRATIO_SPRINT, fMoveBlendRatio);

    CTaskMoveFollowNavMesh* pTaskFollowNavMesh = CreateNavigateTask(iPedIndex, vScrTarget, fMoveBlendRatio, iTime, fTargetRadius, fTargetHeading < DEFAULT_NAVMESH_FINAL_HEADING ? DtoR * fTargetHeading : fTargetHeading);

    if (iTime < -1) {
      scriptAssertf(0, "%s:TASK_FOLLOW_NAVMESH_TO_COORD - Time must be -1 for infinite ", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      return;
    } else if (iTime == 0) {
      scriptAssertf(0, "%s:TASK_FOLLOW_NAVMESH_TO_COORD - You cannot give a task 0 time", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      return;
    }

    if (!pTaskFollowNavMesh)
      return;

    #if __ASSERT
                                                                                                                            if (fTargetHeading<DEFAULT_NAVMESH_FINAL_HEADING && ((iFlags & ENAV_SUPPRESS_EXACT_STOP)!=0))
		{
			Assertf(false, "You cannot specify 'ENAV_SUPPRESS_EXACT_STOP' and have a target-heading");
		}
    #endif

    pTaskFollowNavMesh->SetScriptBehaviourFlags(iFlags, NULL);

    //*****************************************************************************************************
    // The 'fMainTaskTimer' value is used to warp the ped via the parent CTaskComplexControlMovement task

    float fMainTaskTimer = ((float) iTime) / 1000.0f;
    if (fMainTaskTimer > 0.0f)
      fMainTaskTimer += 2.0f;

    //**********************************************************************************
    // Now wrap the task in the CTaskComplexControlMovement parent, and give to the ped

    CTask* pTask = rage_new
    CTaskComplexControlMovement(pTaskFollowNavMesh, NULL, CTaskComplexControlMovement::TerminateOnMovementTask, fMainTaskTimer);
    CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_FOLLOW_NAV_MESH_TO_COORD, "TASK_FOLLOW_NAVMESH_TO_COORD");
  }

  void CommandTaskFollowNavMeshToCoordAdvanced(const int iPedIndex, const Vector3& vTarget, const float fMoveBlendRatio, const int iTime, const float fTargetRadius, const int iFlags, TNavMeshScriptStruct* pNavData, const float fTargetHeading) {
    Assertf(fMoveBlendRatio >= MOVEBLENDRATIO_STILL && fMoveBlendRatio <= MOVEBLENDRATIO_SPRINT, "Invalid 'fMoveBlendRatio' specified in TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED.  Must be in range (%.1f to %.1f) inclusive.  You've passed in %.1f", MOVEBLENDRATIO_STILL, MOVEBLENDRATIO_SPRINT, fMoveBlendRatio);

    //********************
    // Create the task

    if (iTime < -1) {
      scriptAssertf(0, "%s:TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED - Time must be -1 for infinite ", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      return;
    } else if (iTime == 0) {
      scriptAssertf(0, "%s:TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED - You cannot give a task 0 time", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      return;
    }

    CTaskMoveFollowNavMesh* pTaskFollowNavMesh = CreateNavigateTask(iPedIndex, vTarget, fMoveBlendRatio, iTime, fTargetRadius, fTargetHeading < DEFAULT_NAVMESH_FINAL_HEADING ? DtoR * fTargetHeading : fTargetHeading);

    if (!pTaskFollowNavMesh)
      return;

    pTaskFollowNavMesh->SetScriptBehaviourFlags(iFlags, pNavData);

    //*****************************************************************************************************
    // The 'fMainTaskTimer' value is used to warp the ped via the parent CTaskComplexControlMovement task

    float fMainTaskTimer = ((float) iTime) / 1000.0f;
    if (fMainTaskTimer > 0.0f)
      fMainTaskTimer += 2.0f;

    //**********************************************************************************
    // Now wrap the task in the CTaskComplexControlMovement parent, and give to the ped

    CTask* pTask = rage_new
    CTaskComplexControlMovement(pTaskFollowNavMesh, NULL, CTaskComplexControlMovement::TerminateOnMovementTask, fMainTaskTimer);
    CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED, "TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED");
  }

  //***********************************************************
  // CommandTaskNavigateToCoordAdvanced - parameter list:
  //
  // int iPedIndex
  // const scrVector & vScrTarget
  // float fMoveBlendRatio
  // int iTime
  // float fTargetRadius
  // int iFlags
  // TNavMeshScriptStruct navStruct
  //

  void CommandTaskFollowNavMeshToCoordAdvanced_ScrStruct(scrThread::Info& info) {
    //**********************************************************************
    // Extract the parameters from the scrThread::Info structure.  This
    // is necessary as the way of passing structures into script functions

    ASSERT_ONLY(
    static const int iNumParams = 12);
    scriptAssertf(info.ParamCount == iNumParams, "script \'%s\'\nTASK_FOLLOW_NAV_MESH_TO_COORD_ADVANCED - You have passed in the wrong num arguments (%i instead of %i)", CTheScripts::GetCurrentScriptNameAndProgramCounter(), info.ParamCount, iNumParams);

    // Get the ped index
    const int iPedIndex = info.Params[0].Int;
    // Get the target position
    //const float * pFloat = &info.Params[1].Reference->Float;
    const Vector3 vTarget(info.Params[1].Float, info.Params[2].Float, info.Params[3].Float);
    // Get the move blend ratio
    const float fMoveBlendRatio = info.Params[4].Float;
    // Get the time
    int iTime = info.Params[5].Int;
    // Get the target radius
    const float fTargetRadius = info.Params[6].Float;
    // Get the bit-flags
    const int iFlags = info.Params[7].Int;
    // Get the nav data structure
    //void * pStructData = info.Params[6].Reference;
    //TNavMeshScriptStruct * pNavStruct = (TNavMeshScriptStruct*)pStructData;
    TNavMeshScriptStruct NavStruct;
    NavStruct.m_fSlideToCoordHeading = info.Params[8].Float;
    NavStruct.m_fMaxSlopeNavigable = info.Params[9].Float;
    NavStruct.m_fClampMaxSearchDistance = info.Params[10].Float;

    // Final heading
    const float fTargetHeading = info.Params[11].Float;

    CommandTaskFollowNavMeshToCoordAdvanced(iPedIndex, vTarget, fMoveBlendRatio, iTime, fTargetRadius, iFlags, &NavStruct, fTargetHeading);
  }

  // Sets the path-style for this ped, so that they will use climb-overs or not during pathfinding
  void CommandSetPedPathCanUseClimbOvers(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_MAY_CLIMB, Flag);
    }
  }

  // Sets the path-style for this ped, so that they will use ladders or not during pathfinding
  void CommandSetPedPathCanUseLadders(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_MAY_USE_LADDERS, Flag);
    }
  }

  // Sets the path-style for this ped, so that they will use high drop-downs or not during pathfinding
  void CommandSetPedPathCanDropFromHeight(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_MAY_DROP, Flag);
    }
  }

  // Sets the path-style for this ped, so designers can modify how likely it is for a ped to use a climb during navigation
  void CommandSetPedPathClimbCostModifier(int PedIndex, float fModifier) {
    fModifier = Clamp(fModifier, 0.0f, 10.0f);

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetClimbCostModifier(fModifier);
    }
  }

  //
  void CommandSetPedToMayEnterWater(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_MAY_ENTER_WATER, Flag);
    }
  }

  // Sets the path-style for this ped, so that they will use prefer to avoid water or not during pathfinding
  void CommandSetPedPreferToAvoidWater(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_PREFER_TO_AVOID_WATER, Flag);
    }
  }

  // Sets the path-style for this ped, so that they will use prefer to avoid fire during pathfinding
  void CommandSetPedPathAvoidFire(int PedIndex, bool Flag) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      pPedAI->GetNavCapabilities().SetFlag(CPedNavCapabilityInfo::FLAG_AVOID_FIRE, Flag);
    }
  }

  // Needs to be called each frame or it will reset.
  void CommandSetGlobalMinBirdFlightHeight(float fHeight) {
    CWildlifeManager::GetInstance().SetScriptMinBirdHeight(fHeight);
  }

  //****************************************************************************
  //	Retrieves the remaining distance to the target, if a ped is following
  //	a navmesh route in their primary slot.  Returns enumeration value to
  //	indicate whether the navmesh task was found.

  int CommandGetNavMeshRouteDistanceRemaining(int PedIndex, float& fOutDistanceRemaining, int& bThisIsLastRouteSection) {
    fOutDistanceRemaining = 0.0f;
    bThisIsLastRouteSection = false;

    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();
      aiTask* pTask = pPedAI->FindTaskActiveMovementByType(CTaskTypes::TASK_MOVE_FOLLOW_NAVMESH);
      if (pTask) {
        CTaskMoveFollowNavMesh* pNavTask = (CTaskMoveFollowNavMesh*) pTask;

        bThisIsLastRouteSection = pNavTask->GetLastRoutePointIsTarget();

        if (pNavTask->IsFollowingNavMeshRoute() && !pNavTask->GetIsFleeing()) {
          fOutDistanceRemaining = pNavTask->GetDistanceLeftOnCurrentRouteSection(pPed);
          return NAVMESHROUTE_ROUTE_FOUND;
        } else {
          return NAVMESHROUTE_ROUTE_NOT_FOUND;
        }
      }
    }
    return NAVMESHROUTE_TASK_NOT_FOUND;
  }

  int CommandGetNavMeshRouteResult(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();

      // Script task priority must be active
      if (pPedAI->GetTaskManager()->GetTree(PED_TASK_TREE_PRIMARY)->GetActiveTaskPriority() == PED_TASK_PRIORITY_PRIMARY) {
        // The movement task tree must be running its general movement task
        if (pPedAI->GetTaskManager()->GetTree(PED_TASK_TREE_MOVEMENT)->GetActiveTaskPriority() == PED_TASK_MOVEMENT_GENERAL) {
          // A control movement task must be found
          CTaskComplexControlMovement* pControlMovementTask = (CTaskComplexControlMovement*) pPedAI->FindTaskPrimaryByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT);
          // The control movement must have a running movement task
          if (pControlMovementTask && pControlMovementTask->GetRunningMovementTask(pPed)) {
            CTaskMoveFollowNavMesh* pNavTask = (CTaskMoveFollowNavMesh*) pPedAI->GetTaskManager()->GetTree(PED_TASK_TREE_MOVEMENT)->FindTaskByTypeActive(CTaskTypes::TASK_MOVE_FOLLOW_NAVMESH);
            if (pNavTask) {
              if (pNavTask->GetState() == CTaskNavBase::NavBaseState_FollowingPath) {
                return NAVMESHROUTE_ROUTE_FOUND;
              } else if (pNavTask->IsUnableToFindRoute()) {
                return NAVMESHROUTE_ROUTE_NOT_FOUND;
              } else {
                return NAVMESHROUTE_ROUTE_NOT_YET_TRIED;
              }
            }
          }
        }
      }
    }
    return NAVMESHROUTE_TASK_NOT_FOUND;
  }

  bool CommandIsControlledVehicleUnableToGetToRoad(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      CPedIntelligence* pPedAI = pPed->GetPedIntelligence();

      // Script task priority must be active
      if (pPedAI->GetTaskManager()->GetTree(PED_TASK_TREE_PRIMARY)->GetActiveTaskPriority() == PED_TASK_PRIORITY_PRIMARY) {
        // A control vehicle task must be found
        CTaskControlVehicle* pControlVehicleTask = (CTaskControlVehicle*) pPedAI->FindTaskPrimaryByType(CTaskTypes::TASK_CONTROL_VEHICLE);
        if (pControlVehicleTask) {
          return pControlVehicleTask->IsVehicleTaskUnableToGetToRoad();
        }
      }
    }

    return false;
  }

  void CommandTaskGoToCoordAnyMeans(int PedIndex, const scrVector& scrVecCoors, float fMoveBlendRatio, int iVehicleID, bool bUseLongRangeVehiclePathing, s32 iDrivingFlags, float fMaxRangeToShootTargets) {
    Vector3 vTarget(scrVecCoors);
    CVehicle* pVehicle = NULL;
    if (iVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;
    }
    CTask* pTask = rage_new
    CTaskGoToPointAnyMeans(fMoveBlendRatio, vTarget, pVehicle, CTaskGoToPointAnyMeans::ms_fTargetRadius, -1, bUseLongRangeVehiclePathing, iDrivingFlags, fMaxRangeToShootTargets);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_TO_COORD_ANY_MEANS, "TASK_GO_TO_COORD_ANY_MEANS");
  }

  void CommandTaskGoToCoordAnyMeansExtraParams(int PedIndex, const scrVector& scrVecCoors, float fMoveBlendRatio, int iVehicleID, bool bUseLongRangeVehiclePathing, s32 iDrivingFlags, float fMaxRangeToShootTargets, float fExtraVehToTargetDistToPreferVe, float fDriveStraightLineDistance, s32 iExtraFlags, float fWarpTimerMS) {
    Vector3 vTarget(scrVecCoors);
    CVehicle* pVehicle = NULL;
    if (iVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;
    }
    CTask* pTask = rage_new
    CTaskGoToPointAnyMeans(fMoveBlendRatio, vTarget, pVehicle, CTaskGoToPointAnyMeans::ms_fTargetRadius, -1, bUseLongRangeVehiclePathing, iDrivingFlags, fMaxRangeToShootTargets, fExtraVehToTargetDistToPreferVe, fDriveStraightLineDistance, static_cast<u16>(iExtraFlags), -1.0f, fWarpTimerMS);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS, "SCRIPT_TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS");
  }

  void CommandTaskGoToCoordAnyMeansExtraParamsWithCruiseSpeed(int PedIndex, const scrVector& scrVecCoors, float fMoveBlendRatio, int iVehicleID, bool bUseLongRangeVehiclePathing, s32 iDrivingFlags, float fMaxRangeToShootTargets, float fExtraVehToTargetDistToPreferVe, float fDriveStraightLineDistance, s32 iExtraFlags, float fCruiseSpeed, float fTargetArriveDist) {
    Vector3 vTarget(scrVecCoors);
    CVehicle* pVehicle = NULL;
    if (iVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;
    }
    CTask* pTask = rage_new
    CTaskGoToPointAnyMeans(fMoveBlendRatio, vTarget, pVehicle, CTaskGoToPointAnyMeans::ms_fTargetRadius, -1, bUseLongRangeVehiclePathing, iDrivingFlags, fMaxRangeToShootTargets, fExtraVehToTargetDistToPreferVe, fDriveStraightLineDistance, static_cast<u16>(iExtraFlags), fCruiseSpeed, -1.0f, fTargetArriveDist);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED, "TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED");
  }

  #if __BANK
                                                                                                                          bool VerifyDictAndAnim(atHashString dictionaryHash, atHashString animHash, const char * ASSERT_ONLY(scriptName))
	{
		strLocalIndex dictIndex = fwAnimManager::FindSlotFromHashKey(dictionaryHash.GetHash());

		if (scriptVerifyf(dictIndex.Get()>-1, "%s - The dictionary name '%s' is not recognised.", scriptName, dictionaryHash.TryGetCStr()))
		{
			strIndex index = strStreamingEngine::GetInfo().GetModuleMgr().GetModule(fwAnimManager::GetStreamingModuleId())->GetStreamingIndex(dictIndex);
			if (scriptVerifyf(strStreamingEngine::GetInfo().GetStreamingInfo(index)->GetStatus() == STRINFO_LOADED, "%s - The dictionary '%s' is not streamed in.",  scriptName, dictionaryHash.TryGetCStr()))
			{
				const crClip* pClip = fwAnimManager::GetClipIfExistsByDictIndex(dictIndex.Get(), animHash.GetHash());

				if (scriptVerifyf(pClip, "%s - The clip '%s' does not exist in dictionary '%s'", scriptName, animHash.TryGetCStr(), dictionaryHash.TryGetCStr()))
				{
					return true;
				}
			}
		}

		return false;
	}

  #endif //__BANK

  CTaskScriptedAnimation::ePlaybackPriority GetPriorityFromControlFlags(s32 nControlFlags) {
    // TODO - convert to support the new anim priority flags
    CTaskScriptedAnimation::ePlaybackPriority priority = CTaskScriptedAnimation::kPriorityLow;

    if (nControlFlags == AF_PRIORITY_MEDIUM) {
      priority = CTaskScriptedAnimation::kPriorityMid;
    } else if (nControlFlags == AF_PRIORITY_HIGH) {
      priority = CTaskScriptedAnimation::kPriorityHigh;
    }
    return priority;
  }

  void PlayAnimScriptCommand(int CurrCommand, int PedIndex, const char* pAnimName, const char* pAnimDictNameStr, float fBlendInDelta, float fBlendOutDelta, int nTimeToPlay, int nControlFlags, int nIkFlags, bool bAdvanced, const Vector3& initialPosition, const Vector3& initialOrientation, float fStartPhase, EulerAngleOrder RotationOrder, bool phaseControlled, bool bAllowOverrideCloneUpdate) {
    eScriptedAnimFlagsBitSet flags;
    flags.BitSet().SetBits(nControlFlags);

    if (!SCRIPT_VERIFY(pAnimName, "TASK_PLAY_ANIM - Animation name is null")) {
      return;
    }

    if (!SCRIPT_VERIFY(pAnimDictNameStr, "TASK_PLAY_ANIM - Animation dictionary name is null")) {
      return;
    }

    if (bAllowOverrideCloneUpdate && !NetworkInterface::IsGameInProgress()) {
      bAllowOverrideCloneUpdate = false;
      SCRIPT_ASSERT(0, "Don't expect bAllowOverrideCloneUpdate to be set true in SP. Clearing this bool.");
    }

    const strStreamingObjectName pAnimDictName(pAnimDictNameStr);

    CPed* pPed = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != PedIndex) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pPed) {
        return;
      }

      if (pPed->GetIsAttached()) {
        SCRIPT_ASSERT(!flags.BitSet().IsSet(AF_OVERRIDE_PHYSICS), "TASK_PLAY_ANIM - Cannot use AF_OVERRIDE_PHYSICS when ped is attached!");
        SCRIPT_ASSERT(!flags.BitSet().IsSet(AF_TURN_OFF_COLLISION), "TASK_PLAY_ANIM - Cannot set CTaskScriptedAnimation::AF_TURN_OFF_COLLISION when ped is attached!");
        SCRIPT_ASSERT(!flags.BitSet().IsSet(AF_IGNORE_GRAVITY), "TASK_PLAY_ANIM - Cannot set CTaskScriptedAnimation::AF_IGNORE_GRAVITY when ped is attached!");
      }
    } else if (bAllowOverrideCloneUpdate) {
      SCRIPT_ASSERT(0, "In MP don't expect bAllowOverrideCloneUpdate to be set true with null PedIndex!");
      return;
    }

    bool bPartOfASequence = (PedIndex == NULL_IN_SCRIPTING_LANGUAGE);

    //scriptAssertf(strlen(pAnimName)<ANIM_NAMELEN, "%s:TASK_PLAY_ANIM - Animation name is null : %s", CTheScripts::GetCurrentScriptNameAndProgramCounter(), pAnimName);

    if (!bPartOfASequence) {
      BANK_ONLY(VerifyDictAndAnim(pAnimDictName, pAnimName, "TASK_PLAY_ANIM");)
    }

    u32 iBoneMask = BONEMASK_ALL;

    eIkControlFlagsBitSet& ikFlags = reinterpret_cast<eIkControlFlagsBitSet&>(nIkFlags);

    if (flags.BitSet().IsSet(AF_UPPERBODY)) {
      iBoneMask = BONEMASK_UPPERONLY;
    }

    // Currently, always extract the mover and the initial offset when using override physics mode
    if (flags.BitSet().IsSet(AF_OVERRIDE_PHYSICS)) {
      flags.BitSet().Set(AF_USE_MOVER_EXTRACTION, true);
      flags.BitSet().Set(AF_EXTRACT_INITIAL_OFFSET, true);
    }

    CTask* pTask;

    if (nTimeToPlay < 0)
      nTimeToPlay = -1;

    if (bAdvanced) {
      Quaternion rotationQuaternion;
      CScriptEulers::QuaternionFromEulers(rotationQuaternion, initialOrientation, RotationOrder);
      pTask = rage_new
      CTaskScriptedAnimation(pAnimDictName, pAnimName, CTaskScriptedAnimation::kPriorityLow, iBoneMask, fwAnimHelpers::CalcBlendDuration(fBlendInDelta), fwAnimHelpers::CalcBlendDuration(fBlendOutDelta), nTimeToPlay, flags, fStartPhase, initialPosition, rotationQuaternion, false, phaseControlled, ikFlags, bAllowOverrideCloneUpdate, bPartOfASequence);
    } else {
      pTask = rage_new
      CTaskScriptedAnimation(pAnimDictName, pAnimName, CTaskScriptedAnimation::kPriorityLow, iBoneMask, fwAnimHelpers::CalcBlendDuration(fBlendInDelta), fwAnimHelpers::CalcBlendDuration(fBlendOutDelta), nTimeToPlay, flags, fStartPhase, false, phaseControlled, ikFlags, bAllowOverrideCloneUpdate, bPartOfASequence);
    }

    #if !__FINAL
    if (NetworkInterface::IsGameInProgress()) {
      scriptDebugf1("PlayAnimScriptCommand PedIndex %d, pPed %s, pAnimName %s, pAnimDictNameStr %s nControlFlags 0x%x, pTask [ %p ]",
                    PedIndex,
                    pPed ? (pPed->GetNetworkObject() ? pPed->GetNetworkObject()->GetLogName() : pPed->GetModelName()) : "Null pPed",
                    pAnimName,
                    pAnimDictNameStr,
                    nControlFlags,
                    pTask);
    }
    #endif

    if (flags.BitSet().IsSet(AF_USE_ALTERNATIVE_FP_ANIM)) {
      static const char* s_fpsClipSuffixHash = "_FP";
      u32 fpsClipHash = atPartialStringHash(s_fpsClipSuffixHash, atPartialStringHash(pAnimName));
      atHashString finalFpsHash(atFinalizeHash(fpsClipHash));
      static_cast<CTaskScriptedAnimation*>(pTask)->SetFPSClipHash(finalFpsHash);
      if (!bPartOfASequence) {
        BANK_ONLY(VerifyDictAndAnim(pAnimDictName, finalFpsHash, "TASK_PLAY_ANIM");)
      }
    }

    if (bAllowOverrideCloneUpdate) {
      scriptAssertf(!flags.BitSet().IsSet(AF_LOOPING) && !flags.BitSet().IsSet(AF_HOLD_LAST_FRAME), "Don't support AF_LOOPING or AF_HOLD_LAST_FRAME with bAllowOverrideCloneUpdate %d", nControlFlags);

      CTaskScriptedAnimation* pTaskScriptedAnimation = static_cast<CTaskScriptedAnimation*>(pTask);
      if (pPed->IsNetworkClone()) {
        pPed->GetPedIntelligence()->AddLocalCloneTask(pTaskScriptedAnimation, PED_TASK_PRIORITY_PRIMARY);
      } else {
        int iEventPriority = E_PRIORITY_GIVE_PED_TASK;
        CEventGivePedTask event(PED_TASK_PRIORITY_PRIMARY, pTaskScriptedAnimation, false, iEventPriority);
        pPed->GetPedIntelligence()->AddEvent(event);
      }
      return;
    }

    // Nb/ PedIndex == NULL_IN_SCRIPTING_LANGUAGE if the task is part of a sequence
    bool bSecondary = flags.BitSet().IsSet(AF_SECONDARY);

    if (bPartOfASequence && bSecondary) {
      CTask* pTaskSecond = pTask;
      pTask = rage_new
      CTaskAffectSecondaryBehaviour(true, PED_TASK_SECONDARY_PARTIAL_ANIM, pTaskSecond);
    } else if (bSecondary) {
      if (SCRIPT_VERIFY(!bPartOfASequence, "TASK_PLAY_ANIM_SECONDARY* Cant be used in a sequence!")) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
        if (pPed) {
          pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
        }
      }
      return;
    }

    if (!bPartOfASequence && flags.BitSet().IsSet(AF_OVERRIDE_PHYSICS)) {
      //force an ai update post camera so that the anim will be visible next frame (rather than waiting for the next task update)
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
      if (pPed) {
        pPed->SetPedResetFlag(CPED_RESET_FLAG_ForcePostCameraAIUpdate, true);
      }
    }

    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, CurrCommand, "TASK_PLAY_ANIM");
  }

  CTaskScriptedAnimation* FindPrimaryScriptedAnimTask(const CPed* pPed) {
    CTaskScriptedAnimation* pTask = static_cast<CTaskScriptedAnimation*>(pPed->GetPedIntelligence()->FindTaskPrimaryByType(CTaskTypes::TASK_SCRIPTED_ANIMATION));
    return pTask;
  }

  CTaskScriptedAnimation* FindSecondaryScriptedAnimTask(const CPed* pPed) {
    CTask* pTask = pPed->GetPedIntelligence()->GetTaskSecondaryPartialAnim();
    if (pTask && pTask->GetTaskType() == CTaskTypes::TASK_SCRIPTED_ANIMATION) {
      return static_cast<CTaskScriptedAnimation*>(pTask);
    }
    return NULL;
  }

  const crClip* FindClipForScriptCommand(const char* pDictName, const char* pClipName, const char* ASSERT_ONLY(pScriptName)) {
    const crClip* pClip = fwAnimManager::GetClipIfExistsByName(pDictName, pClipName);

    #if __ASSERT
                                                                                                                            if (!pClip)
		{
			const char * pFailReason;
			strLocalIndex dictIndex = strLocalIndex(fwAnimManager::FindSlot(pDictName));
			if (dictIndex.Get()<0)
			{
				pFailReason = " The dictionary does not exist.";
			}
			else
			{
				fwClipDictionaryDef* pDictDef = fwAnimManager::GetSlot(dictIndex);
				if ( !pDictDef)
				{
					pFailReason = " The dictionary is not in the image!";
				}
				else if (!GET_OBJECT(pDictDef))
				{
					pFailReason = " The dictionary is not streamed in.";
				}
				else
				{
					pFailReason = " The clip is not in the dictionary";
				}
			}
			animAssertf (pClip, "(%s) Unable to retrieve clip '%s' from dictionary '%s'.%s", pScriptName, pClipName, pDictName, pFailReason);
		}
    #endif // __ASSERT
    return pClip;
  }

  void CommandTaskPlayAnim(int PedIndex, const char* pAnimDictName, const char* pAnimName, float fBlendInDelta, float fBlendOutDelta, int nTimeToPlay, int nFlags, float startPhase, bool phaseControlled, int ikFlags, bool bAllowOverrideCloneUpdate) {
    if (SCRIPT_VERIFY((nTimeToPlay == -1) || (nTimeToPlay > 0), "TASK_PLAY_ANIM - Time must be -1 for infinite or > 0")) {
      PlayAnimScriptCommand(SCRIPT_TASK_PLAY_ANIM, PedIndex, pAnimName, pAnimDictName, fBlendInDelta, fBlendOutDelta, nTimeToPlay, nFlags, ikFlags, false, VEC3_ZERO, VEC3_ZERO, startPhase, EULER_YXZ, phaseControlled, bAllowOverrideCloneUpdate);
    }
  }

  #if __BANK
                                                                                                                          void VerifySlotData(CTaskScriptedAnimation::ScriptInitSlotData* pData)
	{
		if (pData)
		{
			switch(pData->state.Int)
			{
			case CTaskScriptedAnimation::kStateSingleClip:
				{
					VerifyDictAndAnim(scrDecodeString(pData->dict0.String), scrDecodeString(pData->clip0.String), "TASK_SCRIPTED_ANIMATION");
				}
				break;
			case CTaskScriptedAnimation::kStateBlend:
				{
					VerifyDictAndAnim(scrDecodeString(pData->dict0.String), scrDecodeString(pData->clip0.String), "TASK_SCRIPTED_ANIMATION");
					VerifyDictAndAnim(scrDecodeString(pData->dict1.String), scrDecodeString(pData->clip1.String), "TASK_SCRIPTED_ANIMATION");
					VerifyDictAndAnim(scrDecodeString(pData->dict2.String), scrDecodeString(pData->clip2.String), "TASK_SCRIPTED_ANIMATION");
				}
				break;
			default:
				{

				}
				break;
			}
		}
	}
  #endif //__BANK

  void CommandTaskScriptedAnimation(int pedIndex, int& priorityLowData, int& priorityMidData, int& priorityHighData, float blendInDelta, float BlendOutDelta) {
    CTaskScriptedAnimation::ScriptInitSlotData* priorityLow = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityLowData);
    CTaskScriptedAnimation::ScriptInitSlotData* priorityMid = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityMidData);
    CTaskScriptedAnimation::ScriptInitSlotData* priorityHigh = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityHighData);

    #if __BANK
                                                                                                                            // verify the clips exist, etc
		VerifySlotData(priorityLow);
		VerifySlotData(priorityMid);
		VerifySlotData(priorityHigh);
    #endif //__BANK

    bool bSecondaryTask = false;
    u32 combinedBits = priorityLow->flags.Int | priorityMid->flags.Int | priorityHigh->flags.Int;
    eScriptedAnimFlagsBitSet& combinedFlags = reinterpret_cast<eScriptedAnimFlagsBitSet&>(combinedBits);

    if (combinedFlags.BitSet().IsSet(AF_SECONDARY)) {
      bSecondaryTask = true;
    }

    #if __ASSERT
                                                                                                                            if (NetworkInterface::IsGameInProgress() )
		{
			if( (priorityMid->state.Int!=CTaskScriptedAnimation::kStateEmpty) || (priorityHigh->state.Int!=CTaskScriptedAnimation::kStateEmpty) )
			{
				scriptAssertf(false,"TASK_SCRIPTED_ANIMATION:: Network implementation only handles one slot data in ANIM_DATA& priorityLow.  priorityMid %s, priorityHigh %s",
					(priorityMid->state.Int!=CTaskScriptedAnimation::kStateEmpty)?"Valid":"Empty",
					(priorityHigh->state.Int!=CTaskScriptedAnimation::kStateEmpty)?"Valid":"Empty");
			}
			scriptAssertf( (priorityLow->clip2.String==NULL || priorityLow->weight2.Float==0.0f),"TASK_SCRIPTED_ANIMATION:: Network blended anim implementation only handles first 2 weighted anims. weight2 = %.5f, clip =%s",
							priorityLow->weight2.Float,
							priorityLow->clip2.String?priorityLow->clip2.String:"Null clip string");
		}
    #endif
    // Start the new task
    CTask* pTask = rage_new
    CTaskScriptedAnimation(*priorityLow, *priorityMid, *priorityHigh, blendInDelta, BlendOutDelta);

    // handle the automated FPS alternative clip
    if (combinedFlags.BitSet().IsSet(AF_USE_ALTERNATIVE_FP_ANIM)) {
      if (pTask && priorityLow->state.Int == CTaskScriptedAnimation::kStateSingleClip) {
        static const char* s_fpsClipSuffixHash = "_FP";
        u32 fpsClipHash = atPartialStringHash(s_fpsClipSuffixHash, atPartialStringHash(priorityLow->clip0.String));
        atHashString finalFpsHash(atFinalizeHash(fpsClipHash));
        static_cast<CTaskScriptedAnimation*>(pTask)->SetFPSClipHash(finalFpsHash);
        BANK_ONLY(VerifyDictAndAnim(scrDecodeString(priorityLow->dict0.String), finalFpsHash, "TASK_SCRIPTED_ANIMATION");)
      }
    }

    bool bPartOfASequence = (pedIndex == NULL_IN_SCRIPTING_LANGUAGE);

    if (bPartOfASequence && bSecondaryTask) {
      CTask* pTaskSecond = pTask;
      pTask = rage_new
      CTaskAffectSecondaryBehaviour(true, PED_TASK_SECONDARY_PARTIAL_ANIM, pTaskSecond);
    } else if (bSecondaryTask) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(pedIndex, bSecondaryTask ? CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES : CTheScripts::GUID_ASSERT_FLAGS_ALL);
      if (pPed) {
        pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
      }
      return;
    }

    if (!bPartOfASequence && combinedFlags.BitSet().IsSet(AF_OVERRIDE_PHYSICS)) {
      //force an ai update post camera so that the anim will be visible next frame (rather than waiting for the next task update)
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(pedIndex, bSecondaryTask ? CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES : CTheScripts::GUID_ASSERT_FLAGS_ALL);
      if (pPed) {
        pPed->SetPedResetFlag(CPED_RESET_FLAG_ForcePostCameraAIUpdate, true);
      }
    }

    CScriptPeds::GivePedScriptedTask(pedIndex, pTask, SCRIPT_TASK_PLAY_ANIM, "TASK_SCRIPTED_ANIMATION");
  }

  void CommandPlayEntityScriptedAnimation(int EntityIndex, int& priorityLowData, int& priorityMidData, int& priorityHighData, float blendInDelta, float BlendOutDelta) {
    CTaskScriptedAnimation::ScriptInitSlotData* priorityLow = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityLowData);
    CTaskScriptedAnimation::ScriptInitSlotData* priorityMid = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityMidData);
    CTaskScriptedAnimation::ScriptInitSlotData* priorityHigh = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&priorityHighData);

    #if __BANK
                                                                                                                            // verify the clips exist, etc
		VerifySlotData(priorityLow);
		VerifySlotData(priorityMid);
		VerifySlotData(priorityHigh);
    #endif //__BANK

    // Start the new task
    CPhysical* pEntity = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pEntity) {
      CTask* pTask = rage_new
      CTaskScriptedAnimation(*priorityLow, *priorityMid, *priorityHigh, blendInDelta, BlendOutDelta);
      if (pEntity->GetIsTypeObject()) {
        CObject* pObject = static_cast<CObject*>(pEntity);
        pObject->SetTask(pTask, CObjectIntelligence::OBJECT_TASK_TREE_SECONDARY, CObjectIntelligence::OBJECT_TASK_SECONDARY_ANIM);
      } else if (pEntity->GetIsTypeVehicle()) {
        CVehicle* pVehicle = static_cast<CVehicle*>(pEntity);
        pVehicle->GetIntelligence()->AddTask(VEHICLE_TASK_TREE_SECONDARY, pTask, VEHICLE_TASK_SECONDARY_ANIM, true);
      } else {
        delete pTask;
      }
    }
  }

  void CommandTaskPlayPhoneGestureAnimation(int PedIndex, const char* pAnimDictHash, const char* pAnimName, const char* pFilterName, float fBlendInDuration, float fBlendOutDuration, bool bIsLooping, bool bHoldLastFrame) {
    const CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (SCRIPT_VERIFY(pPed && pPed->GetAnimDirector(), "TASK_PLAY_PHONE_GESTURE_ANIMATION - Invalid ped or ped has no anim director.")) {
      // Look to see if secondary/phone task is running
      CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOBILE_PHONE));
      if (SCRIPT_VERIFY(pTaskMobilePhone, "TASK_PLAY_PHONE_GESTURE_ANIMATION - Ped is not running mobile phone task.")) {
        // Check if clip actually exists
        u32 uDictHashkey = atHashString(pAnimDictHash);
        s32 sAnimDictIndex = fwAnimManager::FindSlotFromHashKey(uDictHashkey).Get();
        u32 uAnimHashKey = atHashString(pAnimName);
        const crClip* pClip = fwAnimManager::GetClipIfExistsByDictIndex(sAnimDictIndex, uAnimHashKey);
        scriptAssertf(pClip, "TASK_PLAY_PHONE_GESTURE_ANIMATION - Animation does not exist. Please ensure dictionary is streamed in! sAnimDictIndex: %d, uAnimHashKey: %d", sAnimDictIndex, uAnimHashKey);

        if (pClip) {
          pTaskMobilePhone->RequestAdditionalSecondaryAnims(pAnimDictHash, pAnimName, pFilterName, fBlendInDuration, fBlendOutDuration, bIsLooping, bHoldLastFrame);
        }
      }
    }
  }

  void CommandTaskStopPhoneGestureAnimation(int PedIndex, float fBlendOutOverride) {
    const CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (SCRIPT_VERIFY(pPed && pPed->GetAnimDirector(), "TASK_STOP_PHONE_GESTURE_ANIMATION - Invalid ped or ped has no anim director.")) {
      CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOBILE_PHONE));
      if (SCRIPT_VERIFY(pTaskMobilePhone, "TASK_STOP_PHONE_GESTURE_ANIMATION - Ped is not running mobile phone task.")) {
        pTaskMobilePhone->ClearAdditionalSecondaryAnimation(fBlendOutOverride);
      }
    }
  }

  bool CommandIsPlayingPhoneGestureAnim(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (SCRIPT_VERIFY(pPed && pPed->GetAnimDirector(), "(IS_PLAYING_PHONE_GESTURE_ANIM - Invalid ped or ped has no anim director.")) {
      CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOBILE_PHONE));
      if (SCRIPT_VERIFY(pTaskMobilePhone, "(IS_PLAYING_PHONE_GESTURE_ANIM - Ped is not running mobile phone task.")) {
        return pTaskMobilePhone->GetIsPlayingAdditionalSecondaryAnim();
      }
    }
    return false;
  }

  float CommandGetPhoneGestureAnimCurrentTime(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (SCRIPT_VERIFY(pPed && pPed->GetAnimDirector(), "GET_PHONE_GESTURE_ANIM_CURRENT_TIME - Invalid ped or ped has no anim director.")) {
      CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOBILE_PHONE));
      if (SCRIPT_VERIFY(pTaskMobilePhone, "GET_PHONE_GESTURE_ANIM_CURRENT_TIME - Ped is not running mobile phone task.")) {
        return pTaskMobilePhone->GetAdditionalSecondaryAnimPhase();
      }
    }
    return -1.0f;
  }

  float CommandGetPhoneGestureAnimTotalTime(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (SCRIPT_VERIFY(pPed && pPed->GetAnimDirector(), "GET_PHONE_GESTURE_ANIM_TOTAL_TIME - Invalid ped or ped has no anim director.")) {
      CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOBILE_PHONE));
      if (SCRIPT_VERIFY(pTaskMobilePhone, "GET_PHONE_GESTURE_ANIM_TOTAL_TIME - Ped is not running mobile phone task.")) {
        return pTaskMobilePhone->GetAdditionalSecondaryAnimDuration();
      }
    }
    return -1.0f;
  }

  void CommandStartAnimPlayback(int EntityIndex, int& data, s32 priority) {
    CTaskScriptedAnimation::ScriptInitSlotData* slotData = reinterpret_cast<CTaskScriptedAnimation::ScriptInitSlotData*>(&data);

    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (slotData->flags.Int & BIT(AF_SECONDARY)) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "START_ANIM_PLAYBACK: Anim task is not playing. Use TASK_SCRIPTED_ANIMATION to begin the task.")) {
        CTaskScriptedAnimation::InitSlotData data(*slotData);
        pTask->StartPlayback(data, GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandStopAnimPlayback(int EntityIndex, s32 priority, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "STOP_ANIM_PLAYBACK: Anim task is not playing.")) {
        pTask->StopPlayback(GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandTaskClipSetClip(int EntityIndex, const char* pAnimDictName, const char* pAnimName, s32 priority, s32 index, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_CLIP: Anim task is not playing. Use TASK_ANIM_START_CLIP or TASK_ANIM_START_BLEND to begin the task.")) {
        const crClip* pClip = FindClipForScriptCommand(pAnimDictName, pAnimName, "TASK_CLIP_SET_CLIP");

        if (pClip) {
          pTask->SetClip(pClip, GetPriorityFromControlFlags(priority), (u8) index);
        }
      }
    }
  }

  void CommandTaskClipSetBlendWeight(int EntityIndex, float weight, s32 priority, s32 index, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_BLEND_WEIGHT: Anim task is not playing. Use TASK_ANIM_START_BLEND to begin the task.")) {

        pTask->SetBlend(weight, GetPriorityFromControlFlags(priority), (u8) index);
      }
    }
  }

  void CommandTaskClipSetFilter(int EntityIndex, const char* pFilterName, s32 priority, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_FILTER: Anim task is not playing. Use TASK_ANIM_START_CLIP or TASK_ANIM_START_BLEND to begin the task.")) {
        pTask->SetFilter(pFilterName, GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandTaskClipSetPhase(int EntityIndex, float phase, s32 priority, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_PHASE: Anim task is not playing. Use TASK_ANIM_START_CLIP or TASK_ANIM_START_BLEND to begin the task.")) {
        pTask->SetPhase(phase, GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandTaskClipSetRate(int EntityIndex, float rate, s32 priority, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_RATE: Anim task is not playing. Use TASK_ANIM_START_CLIP or TASK_ANIM_START_BLEND to begin the task.")) {
        pTask->SetRate(rate, GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandTaskClipSetLooped(int EntityIndex, bool looped, s32 priority, bool secondary = false) {
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      if (secondary) {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
      } else {
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);
      }

      if (SCRIPT_VERIFY(pTask, "TASK_CLIP_SET_LOOPED: Anim task is not playing. Use TASK_ANIM_START_CLIP or TASK_ANIM_START_BLEND to begin the task.")) {
        pTask->SetLooped(looped, GetPriorityFromControlFlags(priority));
      }
    }
  }

  void CommandTaskPlayAnimAdvanced(int PedIndex, const char* pAnimDictName, const char* pAnimName, const scrVector& pos, const scrVector& rot, float fBlendInDelta, float fBlendOutDelta, int nTimeToPlay, int nFlags, float startPhase, int RotOrder, int ikFlags) {
    if (SCRIPT_VERIFY((nTimeToPlay == -1) || (nTimeToPlay > 0), "TASK_PLAY_ANIM - Time must be -1 for infinite or > 0")) {
      // Transform the rotation into x = yaw, y = pitch, z = roll
      // Convert from degrees to radians
      Vector3
      axisRot(DtoR * rot.x, DtoR * rot.y, DtoR * rot.z);

      #if GTA_REPLAY
                                                                                                                              CPed *pPed = NULL;
			if(NULL_IN_SCRIPTING_LANGUAGE!=PedIndex)
			{
				pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
				if(pPed)
				{
					CReplayMgr::RecordWarpedEntity(pPed);
				}
			}
      #endif

      PlayAnimScriptCommand(SCRIPT_TASK_PLAY_ANIM, PedIndex, pAnimName, pAnimDictName, fBlendInDelta, fBlendOutDelta, nTimeToPlay, nFlags, ikFlags, true, Vector3(pos), axisRot, startPhase, static_cast<EulerAngleOrder>(RotOrder), false, false);
    }
  }

  void CommandStopAnimTask(int EntityIndex, const char* pAnimDictName, const char* pAnimName, float fBlendDelta) {
    // Check the object
    CPhysical* pPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(EntityIndex);
    if (pPhysical) {
      // look for an existing scripted animation task running on the ped
      CTaskScriptedAnimation* pTask = NULL;

      pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true);

      if (pTask && pTask->IsPlayingClip(pAnimDictName, pAnimName)) {
        // nothing to see here
      } else {
        //check the secondary task tree
        pTask = entity_commands::FindScriptedAnimTask(EntityIndex, true, false);
        if (pTask && !pTask->IsPlayingClip(pAnimDictName, pAnimName)) {
          pTask = NULL;
        }
      }

      const CTaskSynchronizedScene* pTaskSynchronizedScene = NULL;
      if (!pTask) {
        pTaskSynchronizedScene = entity_commands::FindSynchronizedSceneTask(EntityIndex, pAnimDictName, pAnimName);
        if (!SCRIPT_VERIFY(pTaskSynchronizedScene == NULL, "STOP_ANIM_TASK - Entity is playing the anim through a synced scene, use STOP_SYNCHRONIZED_ENTITY_ANIM instead!")) {
          pTaskSynchronizedScene = NULL;
        }
      }
      if (!pTaskSynchronizedScene) {
        if (SCRIPT_VERIFY(pTask, "STOP_ANIM_TASK - Entity isn't running the anim")) {
          scriptAssertf(fBlendDelta < 0.0f, "Blend out delta must be less than 0.0");
          pTask->BlendOut(fwAnimHelpers::CalcBlendDuration(fBlendDelta));
        }
      }
    }
  }

  void CommandTaskSynchronizedScene(int PedIndex, int sceneId, const char* pAnimDictName, const char* pAnimName, float blendIn, float blendOut, int flags, int ragdollBlockingFlags, float moverBlendDelta, int ikFlags) {
    if (SCRIPT_VERIFY(PedIndex != NULL_IN_SCRIPTING_LANGUAGE, "TASK_SYNCHRONIZED_SCENE cannot be used in a sequence!")) {
      if (SCRIPT_VERIFY(fwAnimDirectorComponentSyncedScene::IsValidSceneId((fwSyncedSceneId) sceneId), "TASK_SYNCHRONIZED_SCENE: Invalid scene id!")) {
        // Get the ped
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
        if (pPed) {
          scriptAssertf(blendIn > 0.0f, "TASK_SYNCHRONIZED_SCENE: blend in delta is (%f) should be > 0.0f. Changing to INSTANT_BLEND_IN_DELTA.", blendIn);
          if (blendIn <= 0.0f)
            blendIn = INSTANT_BLEND_IN_DELTA;

          scriptAssertf(blendOut < 0.0f, "TASK_SYNCHRONIZED_SCENE: blend out delta (%f) should be < 0.0. Changing to INSTANT_BLEND_OUT_DELTA.", blendOut);
          if (blendOut >= 0.0f)
            blendOut = INSTANT_BLEND_OUT_DELTA;

          scriptAssertf(moverBlendDelta > 0.0f, "TASK_SYNCHRONIZED_SCENE: mover blend delta (%f) should be > 0.0f. Changing to INSTANT_BLEND_IN_DELTA!", moverBlendDelta);
          if (moverBlendDelta <= 0.0f)
            moverBlendDelta = INSTANT_BLEND_IN_DELTA;

          if (pPed->GetIsAttached()) {
            // If we are using CTaskUseScenario, it's probably not a problem if we are attached, because it's likely
            // the task itself that has attached the ped.
            CTaskUseScenario* pScenarioTask = static_cast<CTaskUseScenario*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
            if (pScenarioTask) {
              // It would have been expected to me that the running CTaskUseScenario would be aborted and
              // clean up the detachment before CTaskSynchronizedScene starts, so that we would just need
              // to find a way to bypass the attachment check below. For some reason, that doesn't seem to happen,
              // but by calling MakeAbortable(), the task seems to clean up the attachment (and we can still
              // check GetIsAttached() below), in case the abort failed or if there was some other reason the
              // ped was attached. /FF
              pScenarioTask->MakeAbortable(CTask::ABORT_PRIORITY_IMMEDIATE, NULL);
            }
          }

          if (pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle)) {

            // Added by sorr in CL 6438528
            if (pPed->IsLocalPlayer() && pPed->GetPedAudioEntity()) {
              u32 animDictNameHash = atStringHash(pAnimDictName);
              if (animDictNameHash == ATSTRINGHASH("mini@prostitutes@sexlow_veh_first_person", 0x3A68E23A) ||
                  animDictNameHash == ATSTRINGHASH("mini@prostitutes@sexnorm_veh_first_person", 0xF72790A8)) {
                pPed->GetPedAudioEntity()->SetCachedBJVehicle(pPed->GetMyVehicle());
              }
            }
            // Added by sorr in CL 6438528

            Warningf("TASK_SYNCHRONIZED_SCENE: Ped was in vehicle, setting them out of it.");
            pPed->SetPedOutOfVehicle(CPed::PVF_IgnoreSafetyPositionCheck | CPed::PVF_Warp);
          }

          const fwAttachmentEntityExtension* pExtension = pPed->GetAttachmentExtension();
          if (pExtension) {
            fwEntity* entity = pExtension->GetAttachParent();
            if (entity) {
              Warningf("TASK_SYNCHRONIZED_SCENE: Ped was attached.  Detaching them.");
              pPed->DetachFromParent(DETACH_FLAG_IGNORE_SAFE_POSITION_CHECK | DETACH_FLAG_SKIP_CURRENT_POSITION_CHECK);
            }
          }
          scriptAssertf(!pPed->GetIsAttached(), "TASK_SYNCHRONIZED_SCENE: Ped is attached.");

          const crClip* pClip = FindClipForScriptCommand(pAnimDictName, pAnimName, "TASK_SYNCHRONIZED_SCENE");
          if (pClip) {
            eSyncedSceneFlagsBitSet& sceneFlags = reinterpret_cast<eSyncedSceneFlagsBitSet&>(flags);
            eRagdollBlockingFlagsBitSet& ragdollFlags = reinterpret_cast<eRagdollBlockingFlagsBitSet&>(ragdollBlockingFlags);
            eIkControlFlagsBitSet& ikControlFlags = reinterpret_cast<eIkControlFlagsBitSet&>(ikFlags);

            CTaskSynchronizedScene* pTask = rage_new
            CTaskSynchronizedScene(static_cast<fwSyncedSceneId>(sceneId), atPartialStringHash(pAnimName), pAnimDictName, blendIn, blendOut, sceneFlags, ragdollFlags, moverBlendDelta, -1, ikControlFlags);

            #if __DEV
            fwAnimDirectorComponentSyncedScene::RegisterWithSyncedScene(static_cast<fwSyncedSceneId>(sceneId), pPed);
            #endif //__DEV

            CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SYNCHRONIZED_SCENE, "TASK_SYNCHRONIZED_SCENE");
            animEntityDebugf(pPed, "TASK_SYNCHRONIZED_SCENE - %s starting synced scene task", CTheScripts::GetCurrentScriptNameAndProgramCounter());

            // make sure we do an ai update now to sync up next frame
            pPed->SetPedResetFlag(CPED_RESET_FLAG_ForcePostCameraAIUpdate, true);
            pPed->SetPedResetFlag(CPED_RESET_FLAG_AllowUpdateIfNoCollisionLoaded, true);
            if (blendIn == INSTANT_BLEND_IN_DELTA) {
              animDebugf3("[%u:%u] TASK_SYNCHRONIZED_SCENE: forcing a post camera anim update ped %s(%p) for instant blend in of synced scene", fwTimer::GetTimeInMilliseconds(), fwTimer::GetFrameCount(), pPed->GetModelName(), pPed);
              pPed->SetPedResetFlag(CPED_RESET_FLAG_ForcePostCameraAnimUpdate, true);
            }
          }
        }
      }
    }
  }

  int GetActualSequenceId(int Sequence) {
    return CTheScripts::GetCurrentGtaScriptHandler()->GetResourceReferenceFromId(Sequence);
  }

  void CommandOpenSequenceTask(int& Sequence) {
    CScriptResource_SequenceTask sequenceResource;

    Sequence = CTheScripts::GetCurrentGtaScriptHandler()->RegisterScriptResourceAndGetId(sequenceResource);
  }

  void CommandCloseSequenceTask(int Sequence) {
    const int iTaskID = GetActualSequenceId(Sequence);
    if (SCRIPT_VERIFY(iTaskID >= 0, "CLOSE_SEQUENCE_TASK - No free sequences left.")) {
      if (SCRIPT_VERIFY(iTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "CLOSE_SEQUENCE_TASK - Sequence task ID too big")) {
        if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[iTaskID], "CLOSE_SEQUENCE_TASK - Sequence task is already closed")) {
          SCRIPT_ASSERT(!CTaskSequences::ms_TaskSequenceLists[iTaskID].IsEmpty(), "CLOSE_SEQUENCE_TASK - sequence is empty");

          CTaskSequences::ms_bIsOpened[iTaskID] = false;
          CTaskSequences::ms_iActiveSequence = -1;
        }
      }
    }
  }

  void CommandTaskPerformSequence(int PedIndex, int Sequence) {
    const int iTaskID = GetActualSequenceId(Sequence);
    if (SCRIPT_VERIFY(iTaskID >= 0 && iTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "TASK_PERFORM_SEQUENCE - not a valid Sequence task ID")) {
      if (SCRIPT_VERIFY(!CTaskSequences::ms_bIsOpened[iTaskID], "TASK_PERFORM_SEQUENCE - Sequence task is still open")) {
        if (SCRIPT_VERIFY(!CTaskSequences::ms_TaskSequenceLists[iTaskID].IsEmpty(), "TASK_PERFORM_SEQUENCE - sequence task empty")) {
          CTaskUseSequence* pSequenceTask = rage_new
          CTaskUseSequence(iTaskID, Sequence);

          #if __ASSERT
                                                                                                                                  //If assigning a sequence with a prop task in it, make sure the ped has an inventory.

					CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);

					const CTaskList& taskList = pSequenceTask->GetTaskSequenceList()->GetTaskList();
					for (int seqIndex = 0; seqIndex < taskList.GetNumTasks(); ++seqIndex)
					{
						const CTask* pTask = static_cast<const CTask*>(taskList.GetTask(seqIndex));
						if (pTask && pTask->GetTaskType() == CTaskTypes::TASK_USE_SCENARIO)
						{
							const CTaskUseScenario* pScenarioTask = static_cast<const CTaskUseScenario*>(pTask);
							scriptAssertf(!pScenarioTask->GetScenarioInfo().HasProp() || pPed->GetInventory(), "%s: Attempting to assign a ped a task sequence that includes a prop scenario (%s), but the ped(%s) has no inventory", CTheScripts::GetCurrentScriptNameAndProgramCounter(), pScenarioTask->GetScenarioInfo().GetName(), pPed->GetModelName());
						}
					}
          #endif
          CScriptPeds::GivePedScriptedTask(PedIndex, pSequenceTask, SCRIPT_TASK_PERFORM_SEQUENCE, "TASK_PERFORM_SEQUENCE");
        }
      }
    }
  }

  void CommandTaskPerformSequenceLocally(int PedIndex, int Sequence) {
    const int iTaskID = GetActualSequenceId(Sequence);
    if (SCRIPT_VERIFY(iTaskID >= 0 && iTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "TASK_PERFORM_SEQUENCE_LOCALLY - not a valid Sequence task ID")) {
      if (SCRIPT_VERIFY(!CTaskSequences::ms_bIsOpened[iTaskID], "TASK_PERFORM_SEQUENCE_LOCALLY - Sequence task is still open")) {
        if (SCRIPT_VERIFY(!CTaskSequences::ms_TaskSequenceLists[iTaskID].IsEmpty(), "TASK_PERFORM_SEQUENCE_LOCALLY - sequence task empty")) {
          CTaskUseSequence* pTask = rage_new
          CTaskUseSequence(iTaskID);
          CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_PERFORM_SEQUENCE_LOCALLY, "TASK_PERFORM_SEQUENCE_LOCALLY");
        }
      }
    }
  }

  void CommandClearSequenceTask(int& Sequence) {
    CTheScripts::GetCurrentGtaScriptHandler()->RemoveScriptResource(Sequence, false, true, CGameScriptResource::SCRIPT_RESOURCE_SEQUENCE_TASK);
    Sequence = 0;
  }

  void CommandSetSequenceToRepeat(int Sequence, int repeat) {
    const int iSequenceTaskID = GetActualSequenceId(Sequence);
    const int iRepeatMode = repeat;

    if (SCRIPT_VERIFY(iSequenceTaskID == CTaskSequences::ms_iActiveSequence, "SET_SEQUENCE_TO_REPEAT - Using wrong sequence ID")) {
      if (SCRIPT_VERIFY(iSequenceTaskID >= 0 && iSequenceTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "SET_SEQUENCE_TO_REPEAT - Sequence task ID out of range")) {
        if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[iSequenceTaskID], "SET_SEQUENCE_TO_REPEAT - Sequence task needs to be open")) {
          CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].SetRepeatMode(iRepeatMode);
        }
      }
    }
  }

  void CommandSetSequencePreventMigration(int Sequence) {
    const int iSequenceTaskID = GetActualSequenceId(Sequence);

    if (SCRIPT_VERIFY(iSequenceTaskID == CTaskSequences::ms_iActiveSequence, "SET_SEQUENCE_PREVENT_MIGRATION - Using wrong sequence ID")) {
      if (SCRIPT_VERIFY(iSequenceTaskID >= 0 && iSequenceTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "SET_SEQUENCE_TO_REPEAT - Sequence task ID out of range")) {
        if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[iSequenceTaskID], "SET_SEQUENCE_PREVENT_MIGRATION - Sequence task needs to be open")) {
          CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].PreventMigration();
        }
      }
    }
  }

  int CommandGetSequenceProgress(int PedIndex) {
    int iProgress = -1;
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      if (SCRIPT_VERIFY(CPedScriptedTaskRecord::GetStatus(pPed, SCRIPT_TASK_PERFORM_SEQUENCE) >= 0 ||
                        CPedScriptedTaskRecord::GetStatus(pPed, SCRIPT_TASK_PERFORM_SEQUENCE_FROM_PROGRESS) >= 0,
                        "GET_SEQUENCE_PROGRESS - Ped isn't performing sequence")) {
        bool b = false;

        const int iStage1 = CPedScriptedTaskRecord::GetStatus(pPed, SCRIPT_TASK_PERFORM_SEQUENCE);
        if (CPedScriptedTaskRecordData::EVENT_STAGE == iStage1) {
          b = true;
          iProgress = -1;
        }

        const int iStage2 = CPedScriptedTaskRecord::GetStatus(pPed, SCRIPT_TASK_PERFORM_SEQUENCE_FROM_PROGRESS);
        if (CPedScriptedTaskRecordData::EVENT_STAGE == iStage2) {
          b = true;
          iProgress = -1;
        }

        if (!b) {
          if (pPed->GetPedIntelligence()->GetQueriableInterface()->IsTaskPresentAtPriority(CTaskTypes::TASK_USE_SEQUENCE, PED_TASK_PRIORITY_PRIMARY)) {
            iProgress = pPed->GetPedIntelligence()->GetQueriableInterface()->GetSequenceProgressForTaskType(0, CTaskTypes::TASK_USE_SEQUENCE, PED_TASK_PRIORITY_PRIMARY);
          } else {
            scriptAssertf(false, "GET_SEQUENCE_PROGRESS- Ped doesn't have a the correct primary task");
          }
        }
      }
    }
    return iProgress;
  }

  void CommandTaskGetOffBoat(int iPedID, int iTime) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_GET_OFF_BOAT - Time must be -1 for infinite and > 0")) {
      CTaskComplexGetOffBoat* pTaskGetOffBoat = rage_new
      CTaskComplexGetOffBoat(iTime);
      CScriptPeds::GivePedScriptedTask(iPedID, pTaskGetOffBoat, SCRIPT_TASK_GET_OFF_BOAT, "TASK_GET_OFF_BOAT");
    }
  }

  bool CommandGetIsTaskActive(int iPedID, int iCommandType) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      if (pPed->GetPedIntelligence()->GetQueriableInterface()->IsTaskCurrentlyRunning(iCommandType, true)) {
        return true;
      }
    }
    return false;
  }

  int CommandGetActiveVehicleMissionType(int iVehID) {
    const CVehicle* pVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iVehID);
    if (pVehicle) {
      sVehicleMissionParams params;
      s32 iTaskType = CTaskTypes::TASK_INVALID_ID;
      pVehicle->GetIntelligence()->GetActiveTaskInfo(iTaskType, params);

      return CVehicleIntelligence::GetMissionIdentifierFromTaskType(iTaskType, params);
    }

    return MISSION_NONE;
  }

  int CommandGetScriptTaskStatus(int iPedID, int iCommandType) {
    #if __ASSERT
    scriptAssertf(CTheScripts::GetScriptTaskName(iCommandType), "%s GET_SCRIPT_TASK_STATUS: Unknown command type (%d)", CTheScripts::GetCurrentScriptNameAndProgramCounter(), iCommandType);
    #endif // __ASSERT
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      int iTaskStatus = CPedScriptedTaskRecord::GetStatus(pPed, iCommandType);
      if (iTaskStatus == -1) {
        iTaskStatus = CPedScriptedTaskRecordData::MAX_NUM_STAGES;
      }
      return iTaskStatus;
    }
    return (CPedScriptedTaskRecordData::MAX_NUM_STAGES);
  }

  void CommandTaskLeaveAnyVehicle(int iPedID, int iDelayTime, int iFlags) {
    VehicleEnterExitFlags vehicleFlags;
    CTaskVehicleFSM::SetScriptedVehicleEntryExitFlags(vehicleFlags, iFlags);
    if (!vehicleFlags.BitSet().IsSet(CVehicleEnterExitFlags::DontDefaultWarpIfDoorBlocked)) {
      vehicleFlags.BitSet().Set(CVehicleEnterExitFlags::WarpIfDoorBlocked);
    }
    CTask* pTask = rage_new
    CTaskLeaveAnyCar(iDelayTime, vehicleFlags);
    AI_LOG_WITH_ARGS("[Script] - Script %s has created CTaskLeaveAnyCar from SCRIPT_TASK_LEAVE_ANY_VEHICLE at 0x%p for ped with id %i\n", CTheScripts::GetCurrentScriptName(), pTask, iPedID);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_LEAVE_ANY_VEHICLE, "TASK_LEAVE_ANY_VEHICLE");
  }

  void CommandTaskAimGunScripted(int iPedID, int iGunTaskType, bool bDisableBlockingClip, bool bInstantBlendToAim) {
    const CScriptedGunTaskInfo* pScriptedGunTaskInfo = CScriptedGunTaskMetadataMgr::GetScriptedGunTaskInfoByHash((u32) iGunTaskType);

    if (scriptVerifyf(pScriptedGunTaskInfo, "TASK_AIM_GUN_SCRIPTED - scripted gun task info (%u) didn't exist, check metadata", iGunTaskType)) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        SCRIPT_ASSERT(pPed->IsLocalPlayer(), "TASK_AIM_GUN_SCRIPTED - Only supported on the player ped");
        //Position the default target position ahead of the player
        Vector3 targetPos = VEC3V_TO_VECTOR3(pPed->GetTransform().GetPosition()) + (VEC3V_TO_VECTOR3(pPed->GetTransform().GetForward()) * 3.0f);
        CTaskGun* pGunTask = rage_new
        CTaskGun(CWeaponController::WCT_Player, CTaskTypes::TASK_AIM_GUN_SCRIPTED, CWeaponTarget(targetPos));

        if (bDisableBlockingClip)
          pGunTask->GetGunFlags().SetFlag(GF_DisableBlockingClip);

        if (bInstantBlendToAim)
          pGunTask->GetGunFlags().SetFlag(GF_InstantBlendToAim);

        pGunTask->GetGunFlags().SetFlag(GF_ForceAimState);
        pGunTask->GetGunFlags().SetFlag(GF_DisableTorsoIk);
        pGunTask->SetScriptedGunTaskInfoHash(pScriptedGunTaskInfo->GetName().GetHash());
        CScriptPeds::GivePedScriptedTask(iPedID, pGunTask, SCRIPT_TASK_AIM_GUN_SCRIPTED, "TASK_AIM_GUN_SCRIPTED");
      }
    }
  }

  void CommandTaskAimGunScriptedWithTarget(int iPedID, int iTargetPedID, const scrVector& scrVecTargetOrOffset, int iGunTaskType, bool bDisableBlockingClip, bool bForceAim) {
    const CScriptedGunTaskInfo* pScriptedGunTaskInfo = CScriptedGunTaskMetadataMgr::GetScriptedGunTaskInfoByHash((u32) iGunTaskType);

    if (scriptVerifyf(pScriptedGunTaskInfo, "TASK_AIM_GUN_SCRIPTED_WITH_TARGET - scripted gun task info (%u) didn't exist, check metadata", iGunTaskType)) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        CTaskGun* pGunTask;
        Vec3V vTargetOrOffset(scrVecTargetOrOffset);
        const CPed* pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iTargetPedID, CTheScripts::GUID_ASSERT_FLAG_DEAD_CHECK);

        CWeaponController::WeaponControllerType controllerType = CWeaponController::WCT_Fire;
        if (bForceAim) {
          controllerType = CWeaponController::WCT_Aim;
        }

        if (pTargetPed) {
          pGunTask = rage_new
          CTaskGun(controllerType, CTaskTypes::TASK_AIM_GUN_SCRIPTED, CWeaponTarget(pTargetPed, VEC3V_TO_VECTOR3(vTargetOrOffset)));
        } else {
          pGunTask = rage_new
          CTaskGun(controllerType, CTaskTypes::TASK_AIM_GUN_SCRIPTED, CWeaponTarget(VEC3V_TO_VECTOR3(vTargetOrOffset)));
        }

        if (pPed->IsLocalPlayer()) {
          pGunTask->SetFiringPatternHash(FIRING_PATTERN_FULL_AUTO);
        }

        if (bDisableBlockingClip)
          pGunTask->GetGunFlags().SetFlag(GF_DisableBlockingClip);

        pGunTask->GetGunFlags().SetFlag(GF_ForceAimState);
        pGunTask->SetScriptedGunTaskInfoHash(pScriptedGunTaskInfo->GetName().GetHash());
        CScriptPeds::GivePedScriptedTask(iPedID, pGunTask, SCRIPT_TASK_AIM_GUN_SCRIPTED, "TASK_AIM_GUN_SCRIPTED_WITH_TARGET");
      }
    }
  }

  void CommmandUpdateTaskAimGunScriptedTarget(int iPedID, int iTargetPedID, const scrVector& scrVecTargetOrOffset, bool bDisableBlockingClip) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      CTaskAimGunScripted* pAimGunTask = static_cast<CTaskAimGunScripted*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AIM_GUN_SCRIPTED));

      if (SCRIPT_VERIFY(pAimGunTask, "UPDATE_TASK_AIM_GUN_SCRIPTED_TARGET - Ped must be running TASK_AIM_GUN_SCRIPTED")) {
        if (bDisableBlockingClip) {
          pAimGunTask->GetGunFlags().SetFlag(GF_DisableBlockingClip);
        }

        pAimGunTask->GetGunFlags().SetFlag(GF_ForceAimState);

        //Update the target on the gun task
        CTaskGun* pGunTask = static_cast<CTaskGun*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_GUN));
        if (SCRIPT_VERIFY(pGunTask, "UPDATE_TASK_AIM_GUN_SCRIPTED_TARGET - Ped must be running CTaskGun")) {
          Vec3V vTargetOrOffset(scrVecTargetOrOffset);
          const CPed* pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iTargetPedID, CTheScripts::GUID_ASSERT_FLAG_DEAD_CHECK);

          if (pTargetPed) {
            pGunTask->SetTarget(CWeaponTarget(pTargetPed, VEC3V_TO_VECTOR3(vTargetOrOffset)));
          } else {
            pGunTask->SetTarget(CWeaponTarget(VEC3V_TO_VECTOR3(vTargetOrOffset)));
          }
        }
      }
    }
  }

  const char* CommandGetClipSetForScriptedGunTask(s32 iGunTaskType) {
    const CScriptedGunTaskInfo* pScriptedGunTaskInfo = CScriptedGunTaskMetadataMgr::GetScriptedGunTaskInfoByHash((u32) iGunTaskType);
    if (scriptVerifyf(pScriptedGunTaskInfo, "GET_CLIP_SET_FOR_SCRIPTED_GUN_TASK - Scripted gun task info %u didn't exist, check metadata", (u32) iGunTaskType)) {
      return pScriptedGunTaskInfo->GetClipSetName().GetCStr();
    }

    return NULL;
  }

  void CommandSetRopeTrackEntityForGunTask(int iPedID, int iEntityID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (SCRIPT_VERIFY(pPed, "SET_ROPE_TRACK_ENTITY_FOR_SCRIPTED_GUN_TASK - Invalid ped")) {
      CEntity* pEntity = const_cast<CEntity*>(CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID));
      if (SCRIPT_VERIFY(pEntity, "SET_ROPE_TRACK_ENTITY_FOR_SCRIPTED_GUN_TASK - Invalid entity")) {
        CTaskAimGunScripted* pAimTask = static_cast<CTaskAimGunScripted*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AIM_GUN_SCRIPTED));

        if (SCRIPT_VERIFY(pAimTask, "SET_ROPE_TRACK_ENTITY_FOR_SCRIPTED_GUN_TASK - Ped must be running TASK_AIM_GUN_SCRIPTED")) {
          pAimTask->SetRopeOrientationEntity(pEntity);
        }
      }
    }
  }

  void CommandTaskAimGunAtEntity(int iPedID, int iEntityID, int iTime, bool bInstantBlendToAim) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime >= 0), "TASK_AIM_GUN_AT_ENTITY - Time must be -1 for infinite")) {
      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
      if (pEntity) {
        CTaskGun* pTaskGun = rage_new
        CTaskGun(CWeaponController::WCT_Aim, CTaskTypes::TASK_AIM_GUN_ON_FOOT, CWeaponTarget(pEntity), ((float) iTime) / 1000.0f);

        if (iTime == -1) {
          pTaskGun->GetGunFlags().SetFlag(GF_InfiniteDuration);
        } else if (iTime == 0) {
          pTaskGun->GetGunFlags().SetFlag(GF_DisableAiming);
        }

        if (bInstantBlendToAim) {
          pTaskGun->GetGunFlags().SetFlag(GF_InstantBlendToAim);
        }

        CScriptPeds::GivePedScriptedTask(iPedID, pTaskGun, SCRIPT_TASK_AIM_GUN_AT_ENTITY, "TASK_AIM_GUN_AT_ENTITY");
      }
    }
  }

  void CommandTaskTurnPedToFaceEntity(int iPedID, int iEntityID, int iTime) {
    const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK);
    if (pEntity) {
      float fTime;
      if (iTime < 0) {
        fTime = -1.0f;
      } else if (iTime == 0) {
        fTime = 0.0f;
      } else {
        fTime = ((float) iTime) / 1000.0f;
      }
      CTask* pTask = rage_new
      CTaskTurnToFaceEntityOrCoord(pEntity, CTaskMoveAchieveHeading::ms_fHeadingChangeRateFrac, CTaskMoveAchieveHeading::ms_fHeadingTolerance, fTime);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_TURN_PED_TO_FACE_ENTITY, "TASK_TURN_PED_TO_FACE_ENTITY");
    }
  }

  void CommandTaskAimGunAtCoord(int iPedID, const scrVector& scrVecCoors, int iTime, bool bInstantBlendToAim, bool bPlayAimIntro) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime >= 0), "TASK_AIM_GUN_AT_COORD - Time must be -1 for infinite and >= 0")) {
      Vector3 vecAimTarget(scrVecCoors);
      CTaskGun* pTaskGun = rage_new
      CTaskGun(CWeaponController::WCT_Aim, CTaskTypes::TASK_AIM_GUN_ON_FOOT, CWeaponTarget(vecAimTarget), ((float) iTime) / 1000.0f);

      if (iTime == -1) {
        pTaskGun->GetGunFlags().SetFlag(GF_InfiniteDuration);
      } else if (iTime == 0) {
        pTaskGun->GetGunFlags().SetFlag(GF_DisableAiming);
      }

      if (bInstantBlendToAim) {
        pTaskGun->GetGunFlags().SetFlag(GF_InstantBlendToAim);
      }

      if (bPlayAimIntro) {
        pTaskGun->SetPlayAimIntro(true);
      }

      CScriptPeds::GivePedScriptedTask(iPedID, pTaskGun, SCRIPT_TASK_AIM_GUN_AT_COORD, "TASK_AIM_GUN_AT_COORD");
    }
  }

  void CommandTaskShootAtCoord(int iPedID, const scrVector& scrVecCoors, int iTime, int iFiringPatternHash) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SHOOT_GUN_AT_COORD - Time must be -1 for infinite and > 0")) {
      Vector3 TempCoors(scrVecCoors);
      CTask* pTask = NULL;

      if (iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
        if (pPed) {
          if (scriptVerifyf(pPed->GetWeaponManager(), "%s:TASK_SHOOT_GUN_AT_COORD - ped requires a weapon manager", CTheScripts::GetCurrentScriptNameAndProgramCounter())) {
            const CWeaponInfo* pWeaponInfo = CWeaponInfoManager::GetInfo<CWeaponInfo>(pPed->GetWeaponManager()->GetEquippedWeaponHash());

            if (scriptVerifyf(pWeaponInfo, "%s:TASK_SHOOT_GUN_AT_COORD - pWeaponInfo NULL - ped has no equiped weapon info", CTheScripts::GetCurrentScriptNameAndProgramCounter())
                && pWeaponInfo->GetIsThrownWeapon()) {
              pTask = rage_new
              CTaskAimAndThrowProjectile(CWeaponTarget(TempCoors));
            }
          }
        } else {
          return;
        }
      }
      if (pTask == NULL) {
        pTask = rage_new
        CTaskGun(CWeaponController::WCT_Fire, CTaskTypes::TASK_AIM_GUN_ON_FOOT, CWeaponTarget(TempCoors), ((float) iTime) / 1000.0f);

        if (iFiringPatternHash) {
          static_cast<CTaskGun*>(pTask)->SetFiringPatternHash(iFiringPatternHash);
        }

        if (iTime == -1) {
          static_cast<CTaskGun*>(pTask)->GetGunFlags().SetFlag(GF_InfiniteDuration);
        }

      }
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SHOOT_AT_COORD, "TASK_SHOOT_GUN_AT_COORD");
    }
  }

  void CommandTaskShuffleToNextVehicleSeat(int iPedID, int iVehicleID, bool bUseAlternateShuffle) {
    CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    CPed* pPed = NULL;
    if (iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        if (!SCRIPT_VERIFY(pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle) && pPed->GetMyVehicle(), "Ped isn't in a vehicle, need to call IS_PED_IN_ANY_VEHICLE to verify ped is in one") ||
            !SCRIPT_VERIFY(pPed->GetMyVehicle() == pVehicle, "Ped is not in the given vehicle - TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT")) {
          return;
        }
      }
    }

    if (pVehicle) {
      s32 iTargetSeatIndex = -1;
      if (pPed && bUseAlternateShuffle && pVehicle->GetSeatManager() && pVehicle->GetVehicleModelInfo() && pVehicle->GetVehicleModelInfo()->GetModelSeatInfo()) {
        const s32 iCurrentSeatIndex = pVehicle->GetSeatManager()->GetPedsSeatIndex(pPed);
        const s32 iCurrentEntryPointIndex = pVehicle->GetVehicleModelInfo()->GetModelSeatInfo()->GetEntryPointIndexForSeat(iCurrentSeatIndex, pVehicle);
        iTargetSeatIndex = pVehicle->GetVehicleModelInfo()->GetModelSeatInfo()->GetShuffleSeatForSeat(iCurrentEntryPointIndex, iCurrentSeatIndex, true);
      }

      CTask* pTask = rage_new
      CTaskInVehicleSeatShuffle(pVehicle, NULL, true, iTargetSeatIndex);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT, "TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT");
    }
  }

  void CommandClearPedTasks(int iPedID) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed) {
      if (SCRIPT_VERIFY(!pPed->IsInjured(), "Ped is injured, cannot clear all tasks!")) {
        if (pPed->IsNetworkClone()) {
          bool bClearTasksImmediately = false;
          CClearPedTasksEvent::Trigger(pPed, bClearTasksImmediately);
        } else {
          AI_LOG_WITH_ARGS_IF_SCRIPT_OR_PLAYER_PED(pPed, "[Script] - Ped %s has had tasks cleared by script %s\n", AILogging::GetDynamicEntityNameSafe(pPed), CTheScripts::GetCurrentScriptName());
          pPed->GetPedIntelligence()->ClearTasks();
          pPed->SetIsCrouching(false);
        }
        #if __DEV
                                                                                                                                // Record the task in the debug list
				static char stringName[] = "CLEAR_PED_TASKS";
				pPed->GetPedIntelligence()->AddScriptHistoryString(stringName, stringName, CTheScripts::GetCurrentScriptName(), CTheScripts::GetCurrentGtaScriptThread()->GetThreadPC());
        #endif
      }
    }
  }

  void CommandClearPedScriptTaskIfRunningThreatResponseNonTempTask(int iPedID) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed) {
      // If this ped is running a non temporary threat response task, clear their primary (script) task which is dormant
      // [GTAV] B*1605087
      CTask* pNonTempTask = pPed->GetPedIntelligence()->GetTaskAtPriority(PED_TASK_PRIORITY_EVENT_RESPONSE_NONTEMP);
      if (pNonTempTask && pNonTempTask->GetTaskType() == CTaskTypes::TASK_THREAT_RESPONSE) {
        pPed->GetPedIntelligence()->ClearPrimaryTaskAtPriority(PED_TASK_PRIORITY_PRIMARY);
      }
    }
  }

  void CommandClearPedSecondaryTask(int iPedID) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      pPed->GetPedIntelligence()->ClearTasks(false, true);
    }
  }

  void CommandTaskEveryoneLeaveVehicle(int iVehicleID) {
    // this script command is not approved for use in network scripts
    if (SCRIPT_VERIFY(!NetworkInterface::IsGameInProgress(), "%TASK_EVERYONE_LEAVE_VEHICLE - This script command is not allowed in network game scripts!")) {
      CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);
      if (SCRIPT_VERIFY(pVehicle, "TASK_EVERYONE_LEAVE_VEHICLE - Vehicle doesn't exist")) {
        for (int loop = 0; loop < pVehicle->GetSeatManager()->GetMaxSeats(); loop++) {
          CPed* pPedInSeat = pVehicle->GetSeatManager()->GetPedInSeat(loop);
          if (pPedInSeat && !pPedInSeat->IsInjured()) {
            const int iDelay = (loop + 1) * 500 + fwRandom::GetRandomNumberInRange(-100, 100);
            CTask* pTask = rage_new
            CTaskLeaveAnyCar(iDelay);
            AI_LOG_WITH_ARGS("[Script] - Script %s has created CTaskLeaveAnyCar from SCRIPT_TASK_EVERYONE_LEAVE_VEHICLE at 0x%p for ped %s\n", CTheScripts::GetCurrentScriptName(), pTask, AILogging::GetDynamicEntityNameSafe(pPedInSeat));
            const int iPedID = CTheScripts::GetGUIDFromEntity(*pPedInSeat);
            CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_EVERYONE_LEAVE_VEHICLE, "TASK_EVERYONE_LEAVE_VEHICLE");
          }
        }
      }
    }
  }

  // Please keep this in sync with the enumeration in "commands_task.sch"
  enum ESEEK_ENTITY_OFFSET_FLAGS {
    ESEEK_OFFSET_ORIENTATES_WITH_ENTITY = 0x01,
    ESEEK_KEEP_TO_PAVEMENTS = 0x02
  };

  void CommandTaskGotoEntityOffset(int iPedID, int iEntityID, int iTime, float fSeekRadius, float fSeekAngleDeg, float fMoveBlendRatio, int iGotoEntityOffsetFlags) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_GOTO_ENTITY_OFFSET - Time must be -1 for infinite and > 0")) {
      const float fSeekAngle = (DtoR * fSeekAngleDeg);
      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
      if (pEntity) {
        if (NetworkInterface::IsGameInProgress() && (!pEntity->GetIsDynamic() || !((CDynamicEntity*) pEntity)->GetNetworkObject())) {
          scriptAssertf(0, "%s:TASK_GOTO_ENTITY_OFFSET; the entity must be networked or this ped will not be able to migrate properly", CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }

        CTask* pTask = rage_new
        TTaskMoveSeekEntityRadiusAngleOffset(pEntity, iTime);

        CEntitySeekPosCalculatorRadiusAngleOffset seekPosCalculator(fSeekRadius, fSeekAngle);
        ((TTaskMoveSeekEntityRadiusAngleOffset*) pTask)->SetEntitySeekPosCalculator(seekPosCalculator);
        ((TTaskMoveSeekEntityRadiusAngleOffset*) pTask)->SetMoveBlendRatio(fMoveBlendRatio);

        Assertf((iGotoEntityOffsetFlags & ESEEK_OFFSET_ORIENTATES_WITH_ENTITY) == 0, "Can't specify 'ESEEK_OFFSET_ORIENTATES_WITH_ENTITY' with TASK_GOTO_ENTITY_OFFSET, this task always orientates offset with target entity.");

        if ((iGotoEntityOffsetFlags & ESEEK_KEEP_TO_PAVEMENTS) != 0)
          ((TTaskMoveSeekEntityRadiusAngleOffset*) pTask)->SetPreferPavements(true);

        ((TTaskMoveSeekEntityRadiusAngleOffset*) pTask)->SetUseLargerSearchExtents(true);

        CTaskComplexControlMovement* pSimpleControlMovement = rage_new
        CTaskComplexControlMovement(pTask);
        CScriptPeds::GivePedScriptedTask(iPedID, pSimpleControlMovement, SCRIPT_TASK_GOTO_ENTITY_OFFSET, "TASK_GOTO_ENTITY_OFFSET");
      }
    }
  }

  void CommandTaskGotoEntityOffsetXY(int iPedID, int iEntityID, int iTime, float fTargetRadius, float fOffsetX, float fOffsetY, float fMoveBlendRatio, int iGotoEntityOffsetFlags) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_GOTO_ENTITY_OFFSET_XY - Time must be -1 for infinite and > 0")) {
      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
      if (pEntity) {
        if (NetworkInterface::IsGameInProgress() && (!pEntity->GetIsDynamic() || !((CDynamicEntity*) pEntity)->GetNetworkObject())) {
          scriptAssertf(0, "%s:TASK_GOTO_ENTITY_OFFSET_XY; the entity must be networked or this ped will not be able to migrate properly", CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }

        CTask* pTask;

        if ((iGotoEntityOffsetFlags & ESEEK_OFFSET_ORIENTATES_WITH_ENTITY) != 0) {
          pTask = rage_new
          TTaskMoveSeekEntityXYOffsetRotated(pEntity, iTime);
          CEntitySeekPosCalculatorXYOffsetRotated seekPosCalculator(Vector3(fOffsetX, fOffsetY, 0.0f));
          ((TTaskMoveSeekEntityXYOffsetRotated*) pTask)->SetEntitySeekPosCalculator(seekPosCalculator);

          ((TTaskMoveSeekEntityXYOffsetRotated*) pTask)->SetMoveBlendRatio(fMoveBlendRatio);
          ((TTaskMoveSeekEntityXYOffsetRotated*) pTask)->SetTargetRadius(fTargetRadius);
          if ((iGotoEntityOffsetFlags & ESEEK_KEEP_TO_PAVEMENTS) != 0)
            ((TTaskMoveSeekEntityXYOffsetRotated*) pTask)->SetPreferPavements(true);
          ((TTaskMoveSeekEntityXYOffsetRotated*) pTask)->SetUseLargerSearchExtents(true);
        } else {
          pTask = rage_new
          TTaskMoveSeekEntityXYOffsetFixed(pEntity, iTime);
          CEntitySeekPosCalculatorXYOffsetFixed seekPosCalculator(Vector3(fOffsetX, fOffsetY, 0.0f));
          ((TTaskMoveSeekEntityXYOffsetFixed*) pTask)->SetEntitySeekPosCalculator(seekPosCalculator);

          ((TTaskMoveSeekEntityXYOffsetFixed*) pTask)->SetMoveBlendRatio(fMoveBlendRatio);
          ((TTaskMoveSeekEntityXYOffsetFixed*) pTask)->SetTargetRadius(fTargetRadius);
          if ((iGotoEntityOffsetFlags & ESEEK_KEEP_TO_PAVEMENTS) != 0)
            ((TTaskMoveSeekEntityXYOffsetFixed*) pTask)->SetPreferPavements(true);
          ((TTaskMoveSeekEntityXYOffsetFixed*) pTask)->SetUseLargerSearchExtents(true);
        }

        CTaskComplexControlMovement* pSimpleControlMovement = rage_new
        CTaskComplexControlMovement(pTask);
        CScriptPeds::GivePedScriptedTask(iPedID, pSimpleControlMovement, SCRIPT_TASK_GOTO_ENTITY_OFFSET, "TASK_GOTO_ENTITY_OFFSET_XY");
      }
    }
  }

  // Remember to update eScriptInvestigateFlags in commands_task.sch
  enum eScriptInvestigateFlags {
    SIF_DONT_RETURN_TO_ORIGINAL_POSITION = 1,
  };

  void CommandTaskInvestigateCoords(int iPedID, const scrVector& vCoords, int timeToSpendAtSearchPointInMS, int investigateFlags) {
    Vector3 vInvestigationPosition(vCoords);

    // As there's only one flag right now, I'm just passing in a bool to the constructor of CTaskInvestigate. [1/9/2013 mdawe]
    const bool bReturnToPosition = !(investigateFlags & SIF_DONT_RETURN_TO_ORIGINAL_POSITION);
    CTask* pTask = rage_new
    CTaskInvestigate(vInvestigationPosition, NULL, EVENT_SUSPICIOUS_ACTIVITY, timeToSpendAtSearchPointInMS, bReturnToPosition);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_INVESTIGATE_COORDS, "TASK_INVESTIGATE");
  }

  // Remember to update the scripts if you change these flags
  // eScriptLookAtFlags in commands_task.sch
  // Please leave as numbers so its easy to compare with the flags in commands_task.sch
  // i.e dont change 4 to (1<<2)
  enum eScriptLookatFlags {
    SLF_SLOW_TURN_RATE = 1,    // turn the head toward the target slowly
    SLF_FAST_TURN_RATE = 2,    // turn the head toward the target quickly
    SLF_EXTEND_YAW_LIMIT = 4,    // wide yaw head limits
    SLF_EXTEND_PITCH_LIMIT = 8,    // wide pitch head limit
    SLF_WIDEST_YAW_LIMIT = 16,   // widest yaw head limit
    SLF_WIDEST_PITCH_LIMIT = 32,   // widest pitch head limit
    SLF_NARROW_YAW_LIMIT = 64,   // narrow yaw head limits
    SLF_NARROW_PITCH_LIMIT = 128,  // narrow pitch head limit
    SLF_NARROWEST_YAW_LIMIT = 256,  // narrowest yaw head limit
    SLF_NARROWEST_PITCH_LIMIT = 512,  // narrowest pitch head limit
    SLF_USE_TORSO = 1024, // use the torso aswell as the neck and head (currently disabled)
    SLF_WHILE_NOT_IN_FOV = 2048, // keep tracking the target even if they are not in the hard coded FOV
    SLF_USE_CAMERA_FOCUS = 4096, // use the camera as the target
    SLF_USE_EYES_ONLY = 8192, // only track the target with the eyes
    SLF_USE_LOOK_DIR = 16384, // use information in look dir DOF
    SLF_FROM_SCRIPT = 32768, // internal use only
    SLF_USE_REF_DIR_ABSOLUTE = 65536  // use absolute reference direction mode for solver
  };

  int ConvertScriptLookatFlags(int scriptFlags) {
    int lookatFlags = LF_FROM_SCRIPT;
    scriptFlags & SLF_SLOW_TURN_RATE ? lookatFlags |= LF_SLOW_TURN_RATE : 0;
    scriptFlags & SLF_FAST_TURN_RATE ? lookatFlags |= LF_FAST_TURN_RATE : 0;
    scriptFlags & SLF_EXTEND_YAW_LIMIT ? lookatFlags |= LF_WIDEST_YAW_LIMIT : 0;
    scriptFlags & SLF_EXTEND_PITCH_LIMIT ? lookatFlags |= LF_WIDEST_PITCH_LIMIT : 0;
    scriptFlags & SLF_WIDEST_YAW_LIMIT ? lookatFlags |= LF_WIDEST_YAW_LIMIT : 0;
    scriptFlags & SLF_WIDEST_PITCH_LIMIT ? lookatFlags |= LF_WIDEST_PITCH_LIMIT : 0;
    scriptFlags & SLF_NARROW_YAW_LIMIT ? lookatFlags |= LF_NARROW_YAW_LIMIT : 0;
    scriptFlags & SLF_NARROW_PITCH_LIMIT ? lookatFlags |= LF_NARROW_PITCH_LIMIT : 0;
    scriptFlags & SLF_NARROWEST_YAW_LIMIT ? lookatFlags |= LF_NARROWEST_YAW_LIMIT : 0;
    scriptFlags & SLF_NARROWEST_PITCH_LIMIT ? lookatFlags |= LF_NARROWEST_PITCH_LIMIT : 0;
    scriptFlags & SLF_USE_TORSO ? lookatFlags |= LF_USE_TORSO : 0;
    scriptFlags & SLF_WHILE_NOT_IN_FOV ? lookatFlags |= LF_WHILE_NOT_IN_FOV : 0;
    scriptFlags & SLF_USE_CAMERA_FOCUS ? lookatFlags |= LF_USE_CAMERA_FOCUS : 0;
    scriptFlags & SLF_USE_EYES_ONLY ? lookatFlags |= LF_USE_EYES_ONLY : 0;
    scriptFlags & SLF_USE_LOOK_DIR ? lookatFlags |= LF_USE_LOOK_DIR : 0;
    scriptFlags & SLF_USE_REF_DIR_ABSOLUTE ? lookatFlags |= LF_USE_REF_DIR_ABSOLUTE : 0;
    return lookatFlags;
  }

  void CommandTaskLookAtCoord(int iPedID, const scrVector& scrVecCoors, int iTime, int flags, int priority) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_LOOK_AT_COORD - Time must be -1 for infinite and > 0")) {
      int lookatFlags = ConvertScriptLookatFlags(flags);

      if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
        if (SCRIPT_VERIFY(-1 == CTaskSequences::ms_iActiveSequence, "TASK_LOOK_AT_COORD - Sequence opened unexpectedly")) {
          CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
          if (pPed) {
            ikDebugf3("%u TASK_LOOK_AT_COORD(pPed %p %s, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) LookAt",
                      fwTimer::GetFrameCount(), pPed, pPed->GetModelName(), scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority);

            // this script command is not approved for use in network scripts
            Vector3 offset(scrVecCoors);
            pPed->GetIkManager().LookAt(0, 0, iTime, BONETAG_INVALID, &offset, lookatFlags, 500, 500, (CIkManager::eLookAtPriority) priority);
          } else {
            ikDebugf3("%u TASK_LOOK_AT_COORD(iPedID %i, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) Could not find ped!\n%s",
                      fwTimer::GetFrameCount(), iPedID, scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority,
                      CTheScripts::GetCurrentScriptNameAndProgramCounter());
          }
        } else {
          ikDebugf3("%u TASK_LOOK_AT_COORD(iPedID %i, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) Sequence opened unexpectedly!\n%s",
                    fwTimer::GetFrameCount(), iPedID, scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority,
                    CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }
      } else {
        Vector3 offset(scrVecCoors);

        if (SCRIPT_VERIFY(CTaskSequences::ms_iActiveSequence >= 0 && CTaskSequences::ms_iActiveSequence < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "TASK_LOOK_AT_COORD - Sequence task closed")) {
          if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[CTaskSequences::ms_iActiveSequence], "TASK_LOOK_AT_COORD - sequence task closed")) {
            ikDebugf3("%u TASK_LOOK_AT_COORD(iPedID %i, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) CTaskTriggerLookAt",
                      fwTimer::GetFrameCount(), iPedID, scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority);

            CTask* pTask = rage_new
            CTaskTriggerLookAt(NULL, iTime, BONETAG_INVALID, offset, lookatFlags);
            CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].AddTask(pTask);
          } else {
            ikDebugf3("%u TASK_LOOK_AT_COORD(iPedID %i, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) Sequence task closed!\n%s",
                      fwTimer::GetFrameCount(), iPedID, scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority,
                      CTheScripts::GetCurrentScriptNameAndProgramCounter());
          }
        } else {
          ikDebugf3("%u TASK_LOOK_AT_COORD(iPedID %i, scrVecCoors %.3f,%.3f,%.3f, iTime %i, flags %i, priority %i) Sequence task closed!\n%s",
                    fwTimer::GetFrameCount(), iPedID, scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, iTime, flags, priority,
                    CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }
      }
    }
  }

  void CommandTaskLookAtEntity(int iPedID, int iEntityID, int iTime, int flags, int priority) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_LOOK_AT_ENTITY - Time must be -1 for infinite and > 0")) {
      int lookatFlags = ConvertScriptLookatFlags(flags);

      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK);
      if (pEntity) {
        eAnimBoneTag boneTag = BONETAG_INVALID;

        // Use the head bone as the target (if possible)
        if (pEntity->GetIsTypePed()) {
          // Some animals e.g A_C_Stingray dont have a head fall back to the entity position
          s32 boneIdx = -1;
          if (pEntity->GetSkeletonData().ConvertBoneIdToIndex((u16) BONETAG_HEAD, boneIdx)) {
            boneTag = BONETAG_HEAD;
          }
        }

        if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
          if (SCRIPT_VERIFY(-1 == CTaskSequences::ms_iActiveSequence, "TASK_LOOK_AT_ENTITY - Sequence opened unexpectedly")) {
            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK & CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
            if (pPed) {
              scriptAssertf(pPed != pEntity, "TASK_LOOK_AT_ENTITY - Target and source entities are the same (entityId = %d)", iPedID);
              if (NetworkUtils::IsNetworkCloneOrMigrating(pPed)) {
                if (SCRIPT_VERIFY(pEntity->GetIsDynamic(), "TASK_LOOK_AT_ENTITY - Entity for clone is not dynamic!")) {
                  netObject* pNetObj = static_cast<const CDynamicEntity*>(pEntity)->GetNetworkObject();
                  if (SCRIPT_VERIFY(pNetObj, "TASK_LOOK_AT_ENTITY - Entity for clone is not networked!")) {
                    ikDebugf3("%u TASK_LOOK_AT_ENTITY(pPed %p %s, pEntity %p %s, iTime %i, flags %i, priority %i) CSettingOfLookAtEntity",
                              fwTimer::GetFrameCount(), pPed, pPed->GetModelName(), pEntity, pEntity->GetModelName(), iTime, flags, priority);

                    CScriptEntityStateChangeEvent::CSettingOfLookAtEntity parameters(pNetObj, lookatFlags, iTime);
                    CScriptEntityStateChangeEvent::Trigger(pPed->GetNetworkObject(), parameters);
                  } else {
                    ikDebugf3("%u TASK_LOOK_AT_ENTITY(pPed %p %s, pEntity %p %s, iTime %i, flags %i, priority %i) Entity for clone is not networked!\n%s",
                              fwTimer::GetFrameCount(), pPed, pPed->GetModelName(), pEntity, pEntity->GetModelName(), iTime, flags, priority,
                              CTheScripts::GetCurrentScriptNameAndProgramCounter());
                  }
                } else {
                  ikDebugf3("%u TASK_LOOK_AT_ENTITY(pPed %p %s, pEntity %p %s, iTime %i, flags %i, priority %i) Entity for clone is not dynamic!\n%s",
                            fwTimer::GetFrameCount(), pPed, pPed->GetModelName(), pEntity, pEntity->GetModelName(), iTime, flags, priority,
                            CTheScripts::GetCurrentScriptNameAndProgramCounter());
                }
              } else {
                ikDebugf3("%u TASK_LOOK_AT_ENTITY(pPed %p %s, pEntity %p %s, iTime %i, flags %i, priority %i) LookAt",
                          fwTimer::GetFrameCount(), pPed, pPed->GetModelName(), pEntity, pEntity->GetModelName(), iTime, flags, priority);

                pPed->GetIkManager().LookAt(0, pEntity, iTime, boneTag, NULL, lookatFlags, 500, 500, (CIkManager::eLookAtPriority) priority);
              }
            } else {
              ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, pEntity %p %s, iTime %i, flags %i, priority %i) Entity for clone is not dynamic!\n%s",
                        fwTimer::GetFrameCount(), iPedID, pEntity, pEntity->GetModelName(), iTime, flags, priority,
                        CTheScripts::GetCurrentScriptNameAndProgramCounter());
            }
          }
        } else {
          Vector3 offset(0, 0, 0);
          if (SCRIPT_VERIFY(CTaskSequences::ms_iActiveSequence >= 0 && CTaskSequences::ms_iActiveSequence < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "TASK_LOOK_AT_ENTITY - Sequence task closed")) {
            if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[CTaskSequences::ms_iActiveSequence], "TASK_LOOK_AT_ENTITY - sequence task closed")) {
              ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, pEntity %p %s, iTime %i, flags %i, priority %i) CTaskTriggerLookAt",
                        fwTimer::GetFrameCount(), iPedID, pEntity, pEntity->GetModelName(), iTime, flags, priority);

              CTask* pTask = rage_new
              CTaskTriggerLookAt(pEntity, iTime, boneTag, offset, lookatFlags);
              CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].AddTask(pTask);
            } else {
              ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, pEntity %p %s, iTime %i, flags %i, priority %i) Sequence task closed!\n%s",
                        fwTimer::GetFrameCount(), iPedID, pEntity, pEntity->GetModelName(), iTime, flags, priority,
                        CTheScripts::GetCurrentScriptNameAndProgramCounter());
            }
          } else {
            ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, pEntity %p %s, iTime %i, flags %i, priority %i) Sequence task closed!\n%s",
                      fwTimer::GetFrameCount(), iPedID, pEntity, pEntity->GetModelName(), iTime, flags, priority,
                      CTheScripts::GetCurrentScriptNameAndProgramCounter());
          }
        }
      } else {
        ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, iEntityID %i, iTime %i, flags %i, priority %i) Could not find entity!\n%s",
                  fwTimer::GetFrameCount(), iPedID, iEntityID, iTime, flags, priority,
                  CTheScripts::GetCurrentScriptNameAndProgramCounter());
      }
    } else {
      ikDebugf3("%u TASK_LOOK_AT_ENTITY(iPedID %i, iEntityID %i, iTime %i, flags %i, priority %i) Time must be -1 for infinite and > 0!\n%s",
                fwTimer::GetFrameCount(), iPedID, iEntityID, iTime, flags, priority,
                CTheScripts::GetCurrentScriptNameAndProgramCounter());
    }
  }

  void CommandClearLookAt(int iPedID) {
    if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
      if (SCRIPT_VERIFY(-1 == CTaskSequences::ms_iActiveSequence, "TASK_CLEAR_LOOK_AT - Sequence opened unexpectedly")) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
        if (pPed) {
          if (pPed->GetIkManager().IsLooking()) {
            ikDebugf3("%u CLEAR_LOOK_AT(pPed %p %s) AbortLookAt(500)",
                      fwTimer::GetFrameCount(), pPed, pPed->GetModelName());

            pPed->GetIkManager().AbortLookAt(500);
          } else {
            ikDebugf3("%u CLEAR_LOOK_AT(pPed %p %s) Ped is not looking!\n%s",
                      fwTimer::GetFrameCount(), pPed, pPed->GetModelName(),
                      CTheScripts::GetCurrentScriptNameAndProgramCounter());
          }
        } else {
          ikDebugf3("%u CLEAR_LOOK_AT(iPedID %i) Could not find ped!\n%s",
                    fwTimer::GetFrameCount(), iPedID,
                    CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }
      } else {
        ikDebugf3("%u CLEAR_LOOK_AT(iPedID %i) Sequence opened unexpectedly!\n%s",
                  fwTimer::GetFrameCount(), iPedID,
                  CTheScripts::GetCurrentScriptNameAndProgramCounter());
      }
    } else {
      if (SCRIPT_VERIFY(CTaskSequences::ms_iActiveSequence >= 0 && CTaskSequences::ms_iActiveSequence < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "CLEAR_LOOK_AT - Sequence task closed")) {
        if (SCRIPT_VERIFY(CTaskSequences::ms_bIsOpened[CTaskSequences::ms_iActiveSequence], "TASK_CLEAR_LOOK_AT - sequence task closed")) {
          ikDebugf3("%u CLEAR_LOOK_AT(iPedID %i) CTaskClearLookAt",
                    fwTimer::GetFrameCount(), iPedID);

          CTask* pTask = rage_new
          CTaskClearLookAt();
          CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].AddTask(pTask);
        } else {
          ikDebugf3("%u CLEAR_LOOK_AT(iPedID %i) Sequence task closed!\n%s",
                    fwTimer::GetFrameCount(), iPedID,
                    CTheScripts::GetCurrentScriptNameAndProgramCounter());
        }
      } else {
        ikDebugf3("%u CLEAR_LOOK_AT(iPedID %i) Sequence task closed!\n%s",
                  fwTimer::GetFrameCount(), iPedID,
                  CTheScripts::GetCurrentScriptNameAndProgramCounter());
      }
    }
  }

  static u32 giSearchModelIndex = fwModelId::MI_INVALID;
  static CEntity* gpClosestEntity = NULL;
  static float gfClosestDistSq = 0.0f;
  static Vector3 gvSearchPos;

  bool FindNearestEntityCB(CEntity* pEntity, void* UNUSED_PARAM(pData)) {
    C2dEffect* pSpawnPoint = pEntity->GetClosest2dEffect(ET_SPAWN_POINT, gvSearchPos);

    if (!pSpawnPoint) {
      return true;
    }

    if (pEntity->GetModelIndex() == giSearchModelIndex ||
        !CModelInfo::IsValidModelInfo(giSearchModelIndex)) {
      float fDistSq = DistSquared(pEntity->GetTransform().GetPosition(), RCC_VEC3V(gvSearchPos)).Getf();

      if (gpClosestEntity == NULL ||
          gfClosestDistSq > fDistSq) {
        gpClosestEntity = pEntity;
        gfClosestDistSq = fDistSq;
      }
    }
    return true;
  }

  bool CommandIsPedInvestigatingWhistlingEvent(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      CTaskInvestigate* pInvestigateTask = (CTaskInvestigate*) pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_INVESTIGATE);
      if (pInvestigateTask && pInvestigateTask->IsInvestigatingWhistlingEvent()) {
        return true;
      }
    }
    return false;
  }

  void CommandSetInvestigationPosition(int iPedID, const scrVector& scrVecInvestigationPosition) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      CTaskInvestigate* pInvestigateTask = (CTaskInvestigate*) pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_INVESTIGATE);
      if (pInvestigateTask) {
        pInvestigateTask->SetInvestigationPos(scrVecInvestigationPosition);
      }
    }
  }

  void CommandTaskTurnPedToFaceCoord(int iPedID, const scrVector& scrVecCoors, int iTime) {
    float fTime;
    if (iTime < 0) {
      fTime = -1.0f;
    } else if (iTime == 0) {
      fTime = 0.0f;
    } else {
      fTime = ((float) iTime) / 1000.0f;
    }
    CTask* pTask = rage_new
    CTaskTurnToFaceEntityOrCoord(Vector3(scrVecCoors), CTaskMoveAchieveHeading::ms_fHeadingChangeRateFrac, CTaskMoveAchieveHeading::ms_fHeadingTolerance, fTime);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_TURN_PED_TO_FACE_COORD, "TASK_TURN_PED_TO_FACE_COORD");
  }

  void CommandTaskDrivePointRoute(int iPedID, int iVehicleID, float fCruiseSpeed) {
    if (SCRIPT_VERIFY(CTaskMoveFollowPointRoute::ms_pointRoute.GetSize() > 0, "TASK_DRIVE_POINT_ROUTE: Point route didn't have any points in it.")) {
      CTask* pTask;
      CVehicle* pVehicle = NULL;
      if (iVehicleID >= 0) {
        pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
        if (!pVehicle)
          return;
      }
      pTask = rage_new
      CTaskDrivePointRoute(pVehicle, CTaskMoveFollowPointRoute::ms_pointRoute, fCruiseSpeed);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_DRIVE_POINT_ROUTE, "TASK_DRIVE_POINT_ROUTE");
    }
  }

  void GivePedGoToPointAimingTask(
    int iPedID, const CAITarget& goToTarget, const CAITarget& aimAtTarget, float fMoveBlendRatio,
    float TargetRadius, float SlowDistance, bool bShoot, bool bUseNavmesh, int iNavFlags, bool bInstantBlendToAim, int iFiringPatternHash,
    int iScriptTaskType, const char* commandName, const int iTime) {
    #if __ASSERT
                                                                                                                            Vector3 vGoToPosition;
		Vector3 vAimAtPosition;

		if(goToTarget.GetPosition(vGoToPosition))
		{
			scriptAssertf(!vGoToPosition.IsClose(VEC3_ZERO, SMALL_FLOAT), "%s - Goto position is too close to the origin. %s", commandName, CTheScripts::GetCurrentScriptNameAndProgramCounter());
		}

		if(aimAtTarget.GetPosition(vAimAtPosition))
		{
			scriptAssertf(!vAimAtPosition.IsClose(VEC3_ZERO, SMALL_FLOAT), "%s - Aim at position is too close to the origin. %s", commandName, CTheScripts::GetCurrentScriptNameAndProgramCounter());
		}
    #endif

    CTaskGoToPointAiming* pTask = rage_new
    CTaskGoToPointAiming(goToTarget, aimAtTarget, fMoveBlendRatio, bShoot, iTime);
    pTask->SetTargetRadius(TargetRadius);
    pTask->SetSlowDownDistance(SlowDistance);
    pTask->SetUseNavmesh(bUseNavmesh);
    pTask->SetScriptNavFlags(iNavFlags);
    pTask->SetInstantBlendToAim(bInstantBlendToAim);

    if (bShoot) {
      pTask->SetFiringPattern(iFiringPatternHash);
    }

    CScriptPeds::GivePedScriptedTask(iPedID, pTask, iScriptTaskType, commandName);
  }

  void CommandTaskGoToCoordWhileAimingAtCoord(
    int iPedID, const scrVector& scrVecGoToCoord, const scrVector& scrVecAimAtCoord, float fMoveBlendRatio,
    bool bShoot, float TargetRadius, float SlowDistance, bool bUseNavmesh, int iNavFlags, bool bInstantBlendToAim, int iFiringPatternHash) {
    GivePedGoToPointAimingTask(
      iPedID, CAITarget(Vector3(scrVecGoToCoord)), CAITarget(Vector3(scrVecAimAtCoord)), fMoveBlendRatio,
      TargetRadius, SlowDistance, bShoot, bUseNavmesh, iNavFlags, bInstantBlendToAim, iFiringPatternHash, SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD,
      "TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD", -1);
  }

  void CommandTaskGoToCoordWhileAimingAtEntity(
    int iPedID, const scrVector& scrVecGoToCoord, int iAimAtEntityID, float fMoveBlendRatio,
    bool bShoot, float TargetRadius, float SlowDistance, bool bUseNavmesh, int iNavFlags, bool bInstantBlendToAim, int iFiringPatternHash, const int iTime) {
    const CEntity* pAimAtEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iAimAtEntityID);
    if (pAimAtEntity) {
      GivePedGoToPointAimingTask(
        iPedID, CAITarget(Vector3(scrVecGoToCoord)), CAITarget(pAimAtEntity), fMoveBlendRatio,
        TargetRadius, SlowDistance, bShoot, bUseNavmesh, iNavFlags, bInstantBlendToAim, iFiringPatternHash, SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY,
        "TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY", iTime);
    }
  }

  void CommandTaskGoToCoordAndAimAtHatedEntitiesNearCoord(
    int iPedID, const scrVector& scrVecGoToCoord, const scrVector& scrAimNearCoord, float fMoveBlendRatio,
    bool bShoot, float TargetRadius, float SlowDistance, bool bUseNavmesh, int iNavFlags, int iTaskFlags, int iFiringPatternHash) {
    Vector3 vGoToPosition(scrVecGoToCoord);
    scriptAssertf(!vGoToPosition.IsClose(VEC3_ZERO, SMALL_FLOAT), "TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD - Go to position is too close to the origin. %s", CTheScripts::GetCurrentScriptNameAndProgramCounter());

    Vector3 vAimNearPosition(scrAimNearCoord);
    scriptAssertf(!vAimNearPosition.IsClose(VEC3_ZERO, SMALL_FLOAT), "TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD - Aim near position is too close to the origin. %s", CTheScripts::GetCurrentScriptNameAndProgramCounter());

    CTaskGoToPointAiming* pTask = rage_new
    CTaskGoToPointAiming(CAITarget(vGoToPosition), CAITarget(vAimNearPosition), fMoveBlendRatio, bShoot);
    pTask->SetTargetRadius(TargetRadius);
    pTask->SetSlowDownDistance(SlowDistance);
    pTask->SetUseNavmesh(bUseNavmesh);
    pTask->SetAimAtHatedEntitiesNearAimCoord(true);
    pTask->SetScriptNavFlags(iNavFlags);
    pTask->SetConfigFlags(iTaskFlags);

    if (bShoot) {
      pTask->SetFiringPattern(iFiringPatternHash);
    }

    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD,
                                     "TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD");
  }

  void CommandTaskGoToEntityWhileAimingAtCoord(
    int iPedID, int iEntityID, const scrVector& scrVecAimAtCoord, float fMoveBlendRatio,
    bool bShoot, float TargetRadius, float SlowDistance, bool bUseNavmesh, bool bInstantBlendToAim, int iFiringPatternHash) {
    const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
    if (pEntity) {
      GivePedGoToPointAimingTask(
        iPedID, CAITarget(pEntity), CAITarget(Vector3(scrVecAimAtCoord)), fMoveBlendRatio,
        TargetRadius, SlowDistance, bShoot, bUseNavmesh, 0, bInstantBlendToAim, iFiringPatternHash, SCRIPT_TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD,
        "TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD", -1);
    }
  }

  void CommandTaskGoToEntityWhileAimingAtEntity(
    int iPedID, int iGoToEntityID, int iAimAtEntityID, float fMoveBlendRatio,
    bool bShoot, float TargetRadius, float SlowDistance, bool bUseNavmesh, bool bInstantBlendToAim, int iFiringPatternHash) {
    const CEntity* pGoToEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iGoToEntityID);
    const CEntity* pAimAtEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iAimAtEntityID);

    if (pGoToEntity && pAimAtEntity) {
      GivePedGoToPointAimingTask(
        iPedID, CAITarget(pGoToEntity), CAITarget(pAimAtEntity), fMoveBlendRatio,
        TargetRadius, SlowDistance, bShoot, bUseNavmesh, 0, bInstantBlendToAim, iFiringPatternHash, SCRIPT_TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY,
        "TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY", -1);
    }
  }

  void CommandTaskVehicleTempAction(int iPedID, int iVehicleID, int iAction, int iTime) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_VEHICLE_TEMP_ACTION - Time must be -1 for infinite and > 0")) {
      CVehicle* pVehicle = 0;
      CTask* pTask;
      {
        if (iVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
          pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
          if (!pVehicle)
            return;
        }

        if (pVehicle) {
          if (iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK & CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
            if (pPed && NetworkUtils::IsNetworkCloneOrMigrating(pPed)) {
              netObject* pNetObj = pPed->GetNetworkObject();
              if (pNetObj) {
                CScriptEntityStateChangeEvent::CSettingOfTaskVehicleTempAction parameters(pVehicle->GetNetworkObject(), iAction, iTime);
                CScriptEntityStateChangeEvent::Trigger(pNetObj, parameters);
              }
            } else {
              if (NetworkInterface::IsGameInProgress() && iPedID == NULL_IN_SCRIPTING_LANGUAGE) {
                scriptAssertf(0, "TASK_VEHICLE_TEMP_ACTION - temp actions cannot be added to a sequence in MP");
                return;
              }

              pTask = rage_new
              CTaskCarSetTempAction(pVehicle, iAction, iTime);
              CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_TEMP_ACTION, "TASK_VEHICLE_TEMP_ACTION");
            }
          }
        }
      }
    }
  }

  void CommandTaskVehicleMission_Internal(int iPedID, int iVehicleID, int iTargetVehicleID,
                                          int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, float fStraightLineDistance, bool bAllowedToGoAgainstTraffic, Vector3* vTarget) {
    CVehicle* pVehicle = 0;
    if (NULL_IN_SCRIPTING_LANGUAGE != iVehicleID) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!SCRIPT_VERIFY(pVehicle, "TASK_VEHICLE_MISSION needs a valid vehicle!"))
      return;

    CVehicle* pTargetVehicle = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetVehicleID) {
      pTargetVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iTargetVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pTargetVehicle)
        return;
    }

    if (fStraightLineDistance < 0.0f) {
      fStraightLineDistance = sVehicleMissionParams::DEFAULT_SWITCH_TO_STRAIGHT_LINE_DIST;
    }
    fStraightLineDistance = MIN(255.0f, fStraightLineDistance);

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    #if __ASSERT
                                                                                                                            if (iMission == MISSION_CRASH)
		{
			VehicleType vehicleType = pVehicle->GetVehicleType();
			if (vehicleType != VEHICLE_TYPE_PLANE
				&& vehicleType != VEHICLE_TYPE_HELI
				&& vehicleType != VEHICLE_TYPE_BLIMP
				&& vehicleType != VEHICLE_TYPE_AUTOGYRO)
			{
				scriptErrorf("%s:%s Non PLANE, HELI, BLIMP or AUTOGYRO has been told to crash", CTheScripts::GetCurrentScriptNameAndProgramCounter(), "TASK_VEHICLE_MISSION");
			}
		}
    #endif  //__ASSERT

    aiTask* pCarTask = CVehicleIntelligence::GetTaskFromMissionIdentifier(pVehicle, iMission, pTargetVehicle, vTarget, iDrivingStyle, fTargetReached, fStraightLineDistance, fCruiseSpeed, bAllowedToGoAgainstTraffic);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pCarTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_VEHICLE_MISSION");
  }

  void CommandTaskVehicleMission(int iPedID, int iVehicleID, int iTargetVehicleID,
                                 int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, float fStraightLineDistance, bool bDriveAgainstTraffic) {
    CommandTaskVehicleMission_Internal(iPedID, iVehicleID, iTargetVehicleID, iMission, fCruiseSpeed, iDrivingStyle, fTargetReached, fStraightLineDistance, bDriveAgainstTraffic, NULL);
  }

  void CommandTaskVehicleMissionPedTarget_Internal(int iPedID, int iVehicleID, int iTargetPedID,
                                                   int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, float fStraightLineDistance, bool bAllowedToGoAgainstTraffic) {
    CVehicle* pVehicle = 0;
    if (NULL_IN_SCRIPTING_LANGUAGE != iVehicleID) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;
    }

    CPed* pTargetPed = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetPedID) {
      pTargetPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iTargetPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pTargetPed)
        return;
    }

    if (fStraightLineDistance < 0.0f) {
      fStraightLineDistance = sVehicleMissionParams::DEFAULT_SWITCH_TO_STRAIGHT_LINE_DIST;
    }
    fStraightLineDistance = MIN(255.0f, fStraightLineDistance);

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    #if __ASSERT
                                                                                                                            if (iMission == MISSION_CRASH)
		{
			VehicleType vehicleType = pVehicle->GetVehicleType();
			if (vehicleType != VEHICLE_TYPE_PLANE
				&& vehicleType != VEHICLE_TYPE_HELI
				&& vehicleType != VEHICLE_TYPE_BLIMP
				&& vehicleType != VEHICLE_TYPE_AUTOGYRO)
			{
				scriptErrorf("%s:%s Non PLANE, HELI, BLIMP or AUTOGYRO has been told to crash", CTheScripts::GetCurrentScriptNameAndProgramCounter(), "TASK_VEHICLE_MISSION_PED_TARGET");
			}
		}
    #endif  //__ASSERT

    aiTask* pCarTask = CVehicleIntelligence::GetTaskFromMissionIdentifier(pVehicle, iMission, pTargetPed, NULL, iDrivingStyle, fTargetReached, fStraightLineDistance, fCruiseSpeed, bAllowedToGoAgainstTraffic);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pCarTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_VEHICLE_MISSION_PED_TARGET");
  }

  void CommandTaskVehicleMissionPedTarget(int iPedID, int iVehicleID, int iTargetPedID,
                                          int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, float fStraightLineDistance, bool bDriveAgainstTraffic) {
    CommandTaskVehicleMissionPedTarget_Internal(iPedID, iVehicleID, iTargetPedID,
                                                iMission, fCruiseSpeed, iDrivingStyle, fTargetReached, fStraightLineDistance, bDriveAgainstTraffic);
  }

  void CommandTaskVehicleMissionCoorsTarget(int iPedID, int iVehicleID, const scrVector& scrVecCoors,
                                            int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, float fStraightLineDistance, bool bDriveAgainstTraffic) {
    Vector3 VecCoors = Vector3(scrVecCoors);

    CommandTaskVehicleMission_Internal(iPedID, iVehicleID, NULL_IN_SCRIPTING_LANGUAGE,
                                       iMission, fCruiseSpeed, iDrivingStyle, fTargetReached, fStraightLineDistance, bDriveAgainstTraffic, &VecCoors);
  }

  void AddVehicleSubtaskInternal(int iPedID, CTask* pSubTask, const char* ASSERT_ONLY(szTaskName)) {
    if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (!pPed) {
        delete pSubTask;
        return;
      }

      CEventScriptCommand* pEvent = static_cast<CEventScriptCommand*>(pPed->GetPedIntelligence()->GetEventOfType(EVENT_SCRIPT_COMMAND));
      if (!scriptVerifyf(pEvent, "%s:%s - You must have previously given the ped a valid driving task!", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName)) {
        delete pSubTask;
        return;
      }

      aiTask* pTask = pEvent->GetTask();

      if (!scriptVerifyf(pTask && (pTask->GetTaskType() == CTaskTypes::TASK_CONTROL_VEHICLE || pTask->GetTaskType() == CTaskTypes::TASK_CAR_DRIVE_POINT_ROUTE), "%s:%s - You must have previously given the ped a valid driving task!", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName)) {
        delete pSubTask;
        return;
      }
      if (pTask->GetTaskType() == CTaskTypes::TASK_CONTROL_VEHICLE) {
        CTaskControlVehicle* pVehicleTask = static_cast<CTaskControlVehicle*>(pTask);
        pVehicleTask->SetDesiredSubtask(pSubTask);
      } else if (pTask->GetTaskType() == CTaskTypes::TASK_CAR_DRIVE_POINT_ROUTE) {
        CTaskDrivePointRoute* pVehicleTask = static_cast<CTaskDrivePointRoute*>(pTask);
        pVehicleTask->SetDesiredSubtask(pSubTask);
      } else {
        #if __ASSERT
        scriptErrorf( "%s:%s unknown task type", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName);
        #endif
        delete pSubTask;
        return;
      }

    }
      // Add task to last sequence thats open
    else {
      if (!scriptVerifyf(CTaskSequences::ms_iActiveSequence >= 0 && CTaskSequences::ms_iActiveSequence < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "%s:%s Sequence task closed", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName)) {
        delete pSubTask;
        return;
      }
      // Add task to sequence
      if (!scriptVerifyf(CTaskSequences::ms_bIsOpened[CTaskSequences::ms_iActiveSequence], "%s:%s Sequence task closed", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName)) {
        delete pSubTask;
        return;
      }

      aiTask* pTask = CTaskSequences::ms_TaskSequenceLists[CTaskSequences::ms_iActiveSequence].GetLastAddedTask();
      if (scriptVerifyf(pTask && (pTask->GetTaskType() == CTaskTypes::TASK_CONTROL_VEHICLE || pTask->GetTaskType() == CTaskTypes::TASK_CAR_DRIVE_POINT_ROUTE), "%s:%s Last task added to sequence not a valid driving task", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName)) {
        if (pTask->GetTaskType() == CTaskTypes::TASK_CONTROL_VEHICLE) {
          CTaskControlVehicle* pVehicleTask = static_cast<CTaskControlVehicle*>(pTask);
          pVehicleTask->SetDesiredSubtask(pSubTask);
        } else if (pTask->GetTaskType() == CTaskTypes::TASK_CAR_DRIVE_POINT_ROUTE) {
          CTaskDrivePointRoute* pVehicleTask = static_cast<CTaskDrivePointRoute*>(pTask);
          pVehicleTask->SetDesiredSubtask(pSubTask);
        } else {
          #if __ASSERT
          scriptErrorf( "%s:%s unknown task type", CTheScripts::GetCurrentScriptNameAndProgramCounter(), szTaskName);
          #endif
          delete pSubTask;
          return;
        }
      } else {
        delete pSubTask;
      }
    }
  }

  void CommandAddVehicleSubtaskAttackCoord(int iPedID, const scrVector& scrVecCoors) {
    Vector3 vTarget(scrVecCoors);
    CAITarget target(vTarget);
    AddVehicleSubtaskInternal(iPedID, rage_new
    CTaskVehicleCombat(&target), "ADD_VEHICLE_SUBTASK_ATTACK_COORD");
  }

  void CommandAddVehicleSubtaskAttackPed(int iPedID, int iOtherPedID) {
    CPed* pOtherPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iOtherPedID);
    if (!pOtherPed) {
      return;
    }
    CAITarget target(pOtherPed);
    AddVehicleSubtaskInternal(iPedID, rage_new
    CTaskVehicleCombat(&target), "ADD_VEHICLE_SUBTASK_ATTACK_PED");
  }

  void CommandTaskHeliMission(int iPedID, int iVehicleID, int iTargetVehicleID, int iTargetPedID, const scrVector& scrVecCoors,
                              int iMission, float fCruiseSpeed, float fTargetReached, float fHeliOrientation, int iFlightHeight, int iMinHeightAboveTerrain, float fSlowDownDistance, int iHeliFlags) {
    CVehicle* pVehicle = 0;
    Vector3 VecTargetCoors = Vector3(scrVecCoors);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_HELI_MISSION - You must specify a heli to be flown")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;

      if (!SCRIPT_VERIFY(pVehicle->GetVehicleType() == VEHICLE_TYPE_HELI, "TASK_HELI_MISSION - Vehicle to be used (2nd parameter) is not actually a heli"))
        return;
    } else {
      return;
    }

    CPhysical* pPhysical = NULL;
    const CVehicle* pTargetVehicle = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetVehicleID) {
      pTargetVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID);
      if (!pTargetVehicle)
        return;

      pPhysical = const_cast<CVehicle*>(pTargetVehicle);
    }
    const CPed* pTargetPed = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetPedID) {
      pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iTargetPedID);
      if (!pTargetPed)
        return;

      pPhysical = const_cast<CPed*>(pTargetPed);
    }

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    if (fHeliOrientation < 0.0f) {
      fHeliOrientation = -1.0f;
    } else {
      fHeliOrientation = (DtoR * fHeliOrientation);

      // For whatever reason the helicopter AI treats the X axis as 0, so we have to rotate a Y axis heading request by 90
      fHeliOrientation += 90.0f * DtoR;
    }

    aiTask* pHeliTask = CVehicleIntelligence::GetHeliTaskFromMissionIdentifier(pVehicle, iMission, pPhysical, &VecTargetCoors, fTargetReached, fCruiseSpeed, fHeliOrientation, iFlightHeight, iMinHeightAboveTerrain, fSlowDownDistance, iHeliFlags);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pHeliTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_HELI_MISSION");
  }

  void CommandTaskHeliEscort(int iPedID, int iVehicleID, int iTargetVehicleID, const scrVector& offsetVec) {
    CVehicle* pVehicle = 0;
    CVehicle* pTargetVehicle = 0;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_HELI_ESCORT_HELI - You must specify a heli to be flown")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;

      if (!SCRIPT_VERIFY(pVehicle->GetVehicleType() == VEHICLE_TYPE_HELI, "TASK_HELI_ESCORT_HELI - Vehicle to be used (2nd parameter) is not actually a heli"))
        return;
    } else {
      return;
    }

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iTargetVehicleID, "TASK_HELI_ESCORT_HELI - You must specify a heli to be targetted")) {
      pTargetVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iTargetVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pTargetVehicle)
        return;

      if (!SCRIPT_VERIFY(pTargetVehicle->GetVehicleType() == VEHICLE_TYPE_HELI, "TASK_HELI_ESCORT_HELI - Vehicle to be targetted (2nd parameter) is not actually a heli"))
        return;
    } else {
      return;
    }

    Vector3 vOffset(offsetVec);

    sVehicleMissionParams params;
    params.SetTargetEntity(pTargetVehicle);
    params.SetTargetPosition(vOffset);

    CTaskVehicleEscort* pHeliTask = rage_new
    CTaskVehicleEscort(params, CTaskVehicleEscort::VEHICLE_ESCORT_HELI);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pHeliTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_HELI_ESCORT_HELI");
  }

  void CommandTaskPlaneMission(int iPedID, int iVehicleID, int iTargetVehicleID, int iTargetPedID, const scrVector& scrVecCoors,
                               int iMission, float fCruiseSpeed, float fTargetReached, float fOrientation, int iFlightHeight, int iMinHeightAboveTerrain, bool bPrecise) {
    CVehicle* pVehicle = 0;
    Vector3 VecTargetCoors = Vector3(scrVecCoors);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_PLANE_MISSION - You must specify a plane to be flown")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;

      if (!SCRIPT_VERIFY(pVehicle->GetVehicleType() == VEHICLE_TYPE_PLANE, "TASK_PLANE_MISSION - Vehicle to be used (2nd parameter) is not actually a plane"))
        return;
    } else {
      return;
    }

    CPhysical* pPhysical = NULL;
    const CVehicle* pTargetVehicle = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetVehicleID) {
      pTargetVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID);
      if (!pTargetVehicle)
        return;

      pPhysical = const_cast<CVehicle*>(pTargetVehicle);
    }
    const CPed* pTargetPed = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetPedID) {
      pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iTargetPedID);
      if (!pTargetPed)
        return;

      pPhysical = const_cast<CPed*>(pTargetPed);
    }

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    if (fOrientation < 0.0f) {
      fOrientation = -1.0f;
    } else {
      fOrientation = (DtoR * fOrientation);
      // For whatever reason the helicopter AI treats the X axis as 0, so we have to rotate a Y axis heading request by 90
      fOrientation += 90.0f * DtoR;
    }

    aiTask* pPlaneTask = CVehicleIntelligence::GetPlaneTaskFromMissionIdentifier(pVehicle, iMission, pPhysical, &VecTargetCoors, fTargetReached, fCruiseSpeed, fOrientation, iFlightHeight, iMinHeightAboveTerrain, bPrecise);
    if (pPlaneTask) {
      CTask* pTask = rage_new
      CTaskControlVehicle(pVehicle, pPlaneTask);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_PLANE_MISSION");
    }
  }

  void CommandTaskPlaneTaxi(int iPedID, int iVehicleID, const scrVector& scrVecCoors, float fCruiseSpeed, float fTargetReached) {
    CVehicle* pVehicle = 0;
    Vector3 VecTargetCoors = Vector3(scrVecCoors);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_PLANE_TAXI - You must specify a plane to taxi")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;

      if (!SCRIPT_VERIFY(pVehicle->GetVehicleType() == VEHICLE_TYPE_PLANE, "TASK_PLANE_TAXI - Vehicle to be used (2nd parameter) is not actually a plane"))
        return;
    } else {
      return;
    }

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    sVehicleMissionParams params;
    params.SetTargetPosition(VecTargetCoors);
    params.m_fTargetArriveDist = fTargetReached;
    params.m_fCruiseSpeed = fCruiseSpeed;

    aiTask* pGotoTask = rage_new
    CTaskVehicleGoToPointAutomobile(params);

    if (pGotoTask) {
      CTask* pTask = rage_new
      CTaskControlVehicle(pVehicle, pGotoTask);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_PLANE_TAXI");
    }
  }

  void CommandTaskBoatMission(int iPedID, int iVehicleID, int iTargetVehicleID, int iTargetPedID, const scrVector& scrVecCoors,
                              int iMission, float fCruiseSpeed, int iDrivingStyle, float fTargetReached, int iBoatFlags) {
    CVehicle* pVehicle = 0;
    Vector3 VecTargetCoors = Vector3(scrVecCoors);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_BOAT_MISSION - You must specify a boat")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (!pVehicle)
        return;

      if (!SCRIPT_VERIFY(pVehicle->GetVehicleType() == VEHICLE_TYPE_BOAT, "TASK_BOAT_MISSION - Vehicle to be used (2nd parameter) is not actually a boat"))
        return;
    } else {
      return;
    }

    CPhysical* pPhysical = NULL;
    const CVehicle* pTargetVehicle = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetVehicleID) {
      pTargetVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID);
      if (!pTargetVehicle)
        return;

      pPhysical = const_cast<CVehicle*>(pTargetVehicle);
    }
    const CPed* pTargetPed = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != iTargetPedID) {
      pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iTargetPedID);
      if (!pTargetPed)
        return;

      pPhysical = const_cast<CPed*>(pTargetPed);
    }

    if (fTargetReached < 0.0f) {
      fTargetReached = sVehicleMissionParams::DEFAULT_TARGET_REACHED_DIST;
    }

    aiTask* pBoatTask = CVehicleIntelligence::GetBoatTaskFromMissionIdentifier(pVehicle, iMission, pPhysical, &VecTargetCoors, iDrivingStyle, fTargetReached, fCruiseSpeed, iBoatFlags);
    if (pBoatTask) {
      CTask* pTask = rage_new
      CTaskControlVehicle(pVehicle, pBoatTask);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_BOAT_MISSION");
    }
  }

  void CommandTaskVehiclePark(int iPedID, int iVehicleID, const scrVector& vecMoveTarget, float HeadingDegrees, int ParkType, float ToleranceDegrees, bool bKeepLightsOn) {
    CVehicle* pVehicle = NULL;
    Vector3 VecTargetCoors = Vector3(vecMoveTarget);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_VEHICLE_PARK - You must specify a vehicle to be parked")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!pVehicle) {
      return;
    }

    const float HeadingRadians = HeadingDegrees * DtoR;
    Vector3 direction;

    direction.x = -rage::Sinf(HeadingRadians);
    direction.y = rage::Cosf(HeadingRadians);
    direction.z = 0.0f;

    sVehicleMissionParams params;
    params.SetTargetPosition(VecTargetCoors);

    CTaskVehicleParkNew* pParkTask = rage_new
    CTaskVehicleParkNew(params, direction, (CTaskVehicleParkNew::ParkType) ParkType, ToleranceDegrees * DtoR, bKeepLightsOn);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pParkTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_PARK, "TASK_VEHICLE_PARK");
  }

  void CommandTaskStealthKill(int iPedID, int iTargetPedID, int iStealthKillActionResultId, float fDesiredMoveBlendRatio, int iStealthFlags) {
    if (iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
      // Apparently sequence tasks rely on the iPedID to be NULL
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        // Check to make sure the ped has a weapon equipped
        const CWeaponInfo* pEquippedWeaponInfo = pPed->GetWeaponManager()->GetEquippedWeaponInfo();
        if (!SCRIPT_VERIFY(pEquippedWeaponInfo, "TASK_STEALTH_KILL - The ped doesn't have a weapon equipped. Please use GIVE_WEAPON_TO_PED")) {
          return;
        }
      }
    }

    // Ensure target ped is valid
    CPed* pTargetPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iTargetPedID);
    if (!SCRIPT_VERIFY(pTargetPed, "TASK_STEALTH_KILL - The target ped is invalid.")) {
      return;
    }

    // Check to make sure the ped has a weapon equipped
    const CWeaponInfo* pTargetEquippedWeaponInfo = pTargetPed->GetWeaponManager()->GetEquippedWeaponInfo();
    if (!SCRIPT_VERIFY(pTargetEquippedWeaponInfo, "TASK_STEALTH_KILL - The target ped doesn't have a weapon equipped. Please use GIVE_WEAPON_TO_PED")) {
      return;
    }

    // Ensure the stealth kill id is valid
    u32 uActionIdx = 0;
    const CActionResult* pStealthKillActionResult = ACTIONMGR.FindActionResult(uActionIdx, (u32) iStealthKillActionResultId);
    if (!SCRIPT_VERIFY(pStealthKillActionResult, "TASK_STEALTH_KILL - Invalid stealth kill action.")) {
      return;
    }

    CTask* pTask = rage_new
    CTaskStealthKill(pTargetPed, pStealthKillActionResult, fDesiredMoveBlendRatio, iStealthFlags);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_STEALTH_KILL, "TASK_STEALTH_KILL");
  }

  void CommandTaskPlantBomb(int iPedID, const scrVector&, float) {
    if (iPedID != NULL_IN_SCRIPTING_LANGUAGE) {
      // Apparently sequence tasks rely on the iPedID to be NULL
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        // Check to make sure the ped has a weapon equipped
        const CWeaponInfo* pEquippedWeaponInfo = pPed->GetWeaponManager()->GetEquippedWeaponInfo();
        if (!SCRIPT_VERIFY(pEquippedWeaponInfo && pEquippedWeaponInfo->GetProjectileCanBePlaced(), "TASK_PLANT_BOMB - The ped doesn't have a weapon that can be placed. Please use GIVE_WEAPON_TO_PED")) {
          return;
        }
      }
    }

    CTask* pTask = rage_new
    CTaskBomb();
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PLANT_BOMB, "TASK_PLANT_BOMB");
  }

  void CommandTaskSharkCircleCoord(int iPedID, const scrVector& vCoord, float fMBR, float fRadius) {
    CAITarget aiTarget;
    aiTarget.SetPosition(vCoord);
    float fSpeed = CTaskSharkAttack::GetTunables().m_CirclingAngularSpeed;
    CTask* pTask = rage_new
    CTaskComplexControlMovement(rage_new
    CTaskSharkCircle(fMBR, aiTarget, fRadius, fSpeed));
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SHARK_CIRCLE_COORD, "TASK_SHARK_CIRCLE_COORD");
  }

  void CommandTaskSharkCirclePed(int iPedID, int iOtherPedID, float fMBR, float fRadius) {
    CAITarget aiTarget;
    const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedID);
    aiTarget.SetEntity(pOtherPed);
    float fSpeed = CTaskSharkAttack::GetTunables().m_CirclingAngularSpeed;
    CTask* pTask = rage_new
    CTaskComplexControlMovement(rage_new
    CTaskSharkCircle(fMBR, aiTarget, fRadius, fSpeed));
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SHARK_CIRCLE_PED, "TASK_SHARK_CIRCLE_COORD");
  }

  void CommandTaskPlaneGotoPreciseVtol(int iPedID, int iVehicleID, const scrVector& vecMoveTarget, int iFlightHeight, int iMinHeightAboveTerrain, bool bUseDesiredOrientation, float fDesiredOrientation, bool bAutoPilot) {
    CVehicle* pVehicle = NULL;
    Vector3 VecTargetCoors = Vector3(vecMoveTarget);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_PLANE_GOTO_PRECISE_VTOL - You must specify a vehicle to be parked")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);
    }

    if (!pVehicle) {
      return;
    }

    if (!SCRIPT_VERIFY(pVehicle->InheritsFromPlane(), "TASK_PLANE_GOTO_PRECISE_VTOL - Must be used on a plane")) {
      return;
    }

    CPlane* pPlane = static_cast<CPlane*>(pVehicle);
    if (!SCRIPT_VERIFY(pPlane->GetVerticalFlightModeAvaliable(), "TASK_PLANE_GOTO_PRECISE_VTOL - Plane must support VTOL (vertical takeoff and landing)")) {
      return;
    }

    sVehicleMissionParams params;
    params.SetTargetPosition(VecTargetCoors);

    fDesiredOrientation *= DtoR;
    fDesiredOrientation += 90.0f * DtoR;

    fDesiredOrientation = fwAngle::LimitRadianAngleSafe(fDesiredOrientation);

    if (bAutoPilot) {
      params.m_iDrivingFlags.SetFlag(DF_DontTerminateTaskWhenAchieved);
    }

    CTaskVehicleGoToPlane* pPlaneTask = rage_new
    CTaskVehicleGoToPlane(params, iFlightHeight, iMinHeightAboveTerrain, true, bUseDesiredOrientation, fDesiredOrientation);

    if (bAutoPilot) {
      // Apply task directly to the vehicle
      pVehicle->GetIntelligence()->GetTaskManager()->SetTask(VEHICLE_TASK_TREE_PRIMARY, pPlaneTask, VEHICLE_TASK_PRIORITY_PRIMARY, false);
      pVehicle->GetIntelligence()->SetUsingScriptAutopilot(true);

      CVehicleNodeList* pNodeList = pVehicle->GetIntelligence()->GetNodeList();
      if (pNodeList) {
        pNodeList->ClearPathNodes();
      }

      pVehicle->SwitchEngineOn(true);
    } else {
      // Apply task to the ped as part of CTaskControlVehicle
      CTask* pTask = rage_new
      CTaskControlVehicle(pVehicle, pPlaneTask);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_PLANE_GOTO_PRECISE_VTOL");
    }
  }

  void CommandTaskSubmarineGotoAndStop(int iPedID, int iVehicleID, const scrVector& vecMoveTarget, bool bAutoPilot) {
    CVehicle* pVehicle = NULL;
    Vector3 VecTargetCoors = Vector3(vecMoveTarget);

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_PLANE_GOTO_PRECISE_VTOL - You must specify a vehicle to be parked")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);
    }

    if (!pVehicle) {
      return;
    }

    sVehicleMissionParams params;
    params.SetTargetPosition(VecTargetCoors);

    s32 subFlags = 0;
    if (bAutoPilot) {
      params.m_iDrivingFlags.SetFlag(DF_DontTerminateTaskWhenAchieved);
      subFlags = CTaskVehicleGoToSubmarine::SF_HoverAtEnd;
      params.m_fTargetArriveDist = 30.0f;
    }

    CTaskVehicleGoToSubmarine* pSubTask = rage_new
    CTaskVehicleGoToSubmarine(params, subFlags);

    if (bAutoPilot) {
      // Apply task directly to the vehicle
      pVehicle->GetIntelligence()->GetTaskManager()->SetTask(VEHICLE_TASK_TREE_PRIMARY, pSubTask, VEHICLE_TASK_PRIORITY_PRIMARY, false);
      pVehicle->GetIntelligence()->SetUsingScriptAutopilot(true);

      CVehicleNodeList* pNodeList = pVehicle->GetIntelligence()->GetNodeList();
      if (pNodeList) {
        pNodeList->ClearPathNodes();
      }

      pVehicle->SwitchEngineOn(true);
    } else {
      // Apply task to the ped as part of CTaskControlVehicle
      CTask* pTask = rage_new
      CTaskControlVehicle(pVehicle, pSubTask);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_SUBMARINE_GOTO_AND_STOP");
    }
  }

  void CommandClearDefaultPrimaryTask(int iPedID) {
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "CLEAR_DEFAULT_PRIMARY_TASK - Can't have a NULL ped")) {
      if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID)) {
        AI_LOG_WITH_ARGS("[Script] - Script %s has restarted the default primary task for ped %s", CTheScripts::GetCurrentScriptName(), AILogging::GetDynamicEntityNameSafe(pPed));
        pPed->GetPedIntelligence()->AddTaskDefault(pPed->ComputeDefaultTask(*pPed));
      }
    }
  }

  void CommandClearPrimaryVehicleTask(int iVehicleID) {
    CVehicle* pVehicle = NULL;
    pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);

    if (SCRIPT_VERIFYF(pVehicle, "CLEAR_PRIMARY_VEHICLE_TASK - Could not find vehicle with provided ID %i", iVehicleID)) {
      if (pVehicle->GetIntelligence() && pVehicle->GetIntelligence()->GetTaskManager()) {
        pVehicle->GetIntelligence()->GetTaskManager()->ClearTask(VEHICLE_TASK_TREE_PRIMARY, VEHICLE_TASK_PRIORITY_PRIMARY);
      }
    }
  }

  void CommandClearVehicleCrashTask(int iVehicleID) {
    CVehicle* pVehicle = NULL;
    pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);

    if (SCRIPT_VERIFYF(pVehicle, "CLEAR_VEHICLE_CRASH_TASK - Could not find vehicle with provided ID %i", iVehicleID)) {
      if (pVehicle->GetIntelligence() && pVehicle->GetIntelligence()->GetTaskManager()) {
        pVehicle->GetIntelligence()->GetTaskManager()->ClearTask(VEHICLE_TASK_TREE_PRIMARY, VEHICLE_TASK_PRIORITY_CRASH);
      }
    }
  }

  void CommandTaskVehicleEscort(int iPedID, int iVehicleID, int iEscortEntityID, int iEscortType, float fCruiseSpeed, int iDrivingFlags, float fCustomOffset, int iMinHeightAboveTerrain, float fStraightLineDist) {
    CVehicle* pVehicle = NULL;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_VEHICLE_ESCORT - You must specify a vehicle to drive")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!pVehicle) {
      return;
    }

    CEntity* pEntity = CTheScripts::GetEntityToModifyFromGUID<CEntity>(iEscortEntityID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!pEntity) {
      return;
    }

    sVehicleMissionParams params;
    params.m_iDrivingFlags = iDrivingFlags;
    params.SetTargetEntity(pEntity);
    params.m_fCruiseSpeed = fCruiseSpeed;

    if (iMinHeightAboveTerrain < 0) {
      iMinHeightAboveTerrain = CTaskVehicleGoToHelicopter::DEFAULT_MIN_HEIGHT_ABOVE_TERRAIN;
    }
    CTaskVehicleEscort* pEscortTask = rage_new
    CTaskVehicleEscort(params, (CTaskVehicleEscort::VehicleEscortType) iEscortType, fCustomOffset, iMinHeightAboveTerrain, fStraightLineDist);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pEscortTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_VEHICLE_ESCORT");
  }

  void CommandTaskVehicleFollow(int iPedID, int iVehicleID, int iFollowEntityID, float fCruiseSpeed, int iDrivingFlags, int iFollowDistance) {
    CVehicle* pVehicle = NULL;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_VEHICLE_FOLLOW - You must specify a vehicle to drive")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!pVehicle) {
      return;
    }

    CEntity* pEntity = CTheScripts::GetEntityToModifyFromGUID<CEntity>(iFollowEntityID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!pEntity) {
      return;
    }

    sVehicleMissionParams params;
    params.m_iDrivingFlags = iDrivingFlags;
    params.SetTargetEntity(pEntity);
    params.m_fCruiseSpeed = fCruiseSpeed;

    CTaskVehicleFollow* pFollowTask = rage_new
    CTaskVehicleFollow(params, iFollowDistance);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pFollowTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_VEHICLE_FOLLOW");
  }

  void CommandTaskHeliProtect(int iPedID, int iVehicleID, int iEscortEntityID, float fCruiseSpeed, int iDrivingFlags, float fCustomOffset, int iMinHeightAboveTerrain, int iHeliFlags) {
    CVehicle* pVehicle = NULL;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_VEHICLE_HELI_PROTECT - You must specify a vehicle to drive")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    if (!pVehicle) {
      return;
    }

    CEntity* pEntity = CTheScripts::GetEntityToModifyFromGUID<CEntity>(iEscortEntityID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!pEntity) {
      return;
    }

    sVehicleMissionParams params;
    params.m_iDrivingFlags = iDrivingFlags;
    params.SetTargetEntity(pEntity);
    params.m_fCruiseSpeed = fCruiseSpeed;

    if (iMinHeightAboveTerrain < 0) {
      iMinHeightAboveTerrain = CTaskVehicleGoToHelicopter::DEFAULT_MIN_HEIGHT_ABOVE_TERRAIN;
    }
    CTask* pHeliTask = rage_new
    CTaskVehicleHeliProtect(params, fCustomOffset, iMinHeightAboveTerrain, iHeliFlags);
    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pHeliTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_MISSION, "TASK_VEHICLE_HELI_PROTECT");
  }

  void CommandTaskVehicleChase(int iPedID, int iTargetID) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (!SCRIPT_VERIFY(pPed, "TASK_VEHICLE_CHASE - The ped is invalid.")) {
      return;
    }

    //Ensure the ped is in a vehicle.
    if (!SCRIPT_VERIFY(pPed->GetVehiclePedInside(), "TASK_VEHICLE_CHASE - The ped is not in a vehicle.")) {
      return;
    }

    //Ensure the target is valid.
    CPed* pTarget = CTheScripts::GetEntityToModifyFromGUID<CPed>(iTargetID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pTarget, "TASK_VEHICLE_CHASE - The target is invalid.")) {
      return;
    }

    //Ensure the target is in a vehicle.
    if (!SCRIPT_VERIFY(pTarget->GetVehiclePedInside(), "TASK_VEHICLE_CHASE - The target is not in a vehicle.")) {
      return;
    }

    //Create the task.
    CTask* pTask = rage_new
    CTaskVehicleChase(pTarget);

    //Give the task to the ped.
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_CHASE, "TASK_VEHICLE_CHASE");
  }

  void CommandSetTaskVehicleChaseBehaviorFlag(int iPedID, int iFlag, bool bValue) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (!SCRIPT_VERIFY(pPed, "SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG - The ped is invalid.")) {
      return;
    }

    //Ensure the vehicle chase task is valid.
    CTaskVehicleChase* pTask = static_cast<CTaskVehicleChase*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_VEHICLE_CHASE));
    if (!SCRIPT_VERIFY(pTask, "SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG - The task is invalid.")) {
      return;
    }

    //Change the behavior flag.
    pTask->ChangeBehaviorFlag(CTaskVehicleChase::Overrides::Script, (CTaskVehicleChase::BehaviorFlags) iFlag, bValue);
  }

  void CommandSetTaskVehicleChaseIdealPursuitDistance(int iPedID, float fIdealPursuitDistance) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (!SCRIPT_VERIFY(pPed, "SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE - The ped is invalid.")) {
      return;
    }

    //Ensure the vehicle chase task is valid.
    CTaskVehicleChase* pTask = static_cast<CTaskVehicleChase*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_VEHICLE_CHASE));
    if (!SCRIPT_VERIFY(pTask, "SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE - The task is invalid.")) {
      return;
    }

    //Set the ideal distance.
    pTask->SetIdealDistanceForPursue(CTaskVehicleChase::Overrides::Script, fIdealPursuitDistance);
  }

  void CommandTaskPlaneChase(int iPedID, int iTargetID, const scrVector& vTargetOffset) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pPed, "TASK_PLANE_CHASE - The ped is invalid.")) {
      return;
    }

    //Ensure the target is valid.
    CEntity const* pTarget = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iTargetID);
    if (!SCRIPT_VERIFY(pTarget, "TASK_PLANE_CHASE - The target is invalid.")) {
      return;
    }

    CAITarget target;
    target.SetEntityAndOffsetUnlimited(pTarget, vTargetOffset);
    CTask* pTask = rage_new
    CTaskPlaneChase(target);

    //Give the task to the ped.
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PLANE_CHASE, "TASK_PLANE_CHASE");
  }

  void CommandTaskPlaneLand(int iPedID, int iVehicleID, const scrVector& vRunWayStart, const scrVector& vRunWayEnd) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (!SCRIPT_VERIFY(pPed, "TASK_PLANE_LAND - The ped is invalid.")) {
      return;
    }

    CVehicle* pVehicle = NULL;
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_PLANE_LAND - You must specify a vehicle!")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);
    }

    if (!pVehicle) {
      return;
    }

    //Create the task.
    CTask* pTaskVehicleLandPlane = rage_new
    CTaskVehicleLandPlane((Vec3V) vRunWayStart, (Vec3V) vRunWayEnd);
    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pTaskVehicleLandPlane);

    //Give the task to the ped.
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PLANE_LAND, "TASK_PLANE_CHASE");
  }

  void CommandTaskHeliChase(int iPedID, int iTargetID, const scrVector& vTargetOffset) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pPed, "TASK_HELI_CHASE - The ped is invalid.")) {
      return;
    }

    //Ensure the target is valid.
    const CEntity* pTarget = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iTargetID);
    if (!SCRIPT_VERIFY(pTarget, "TASK_HELI_CHASE - The target is invalid.")) {
      return;
    }

    //Create the task.
    CTask* pTask = rage_new
    CTaskHeliChase(CAITarget(pTarget), (Vec3V) vTargetOffset);

    //Give the task to the ped.
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_HELI_CHASE, "TASK_HELI_CHASE");
  }

  void CommandSetTaskHeliChaseTargetOffset(int iPedID, const scrVector& vTargetOffset) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pPed, "SET_TASK_HELI_CHASE_TARGET_OFFSET - The ped is invalid.")) {
      return;
    }

    //Ensure the vehicle chase task is valid.
    CTaskHeliChase* pTask = static_cast<CTaskHeliChase*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_HELI_CHASE));
    if (!SCRIPT_VERIFY(pTask, "SET_TASK_HELI_CHASE_TARGET_OFFSET - The task is invalid.")) {
      return;
    }

    //Set the target offset.
    pTask->SetTargetOffset((Vec3V) vTargetOffset);
  }

  void CommandSetTaskHeliChaseTargetOffsetWorldSpace(int iPedID, bool bTargetOffsetWorldSpace) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pPed, "SET_TASK_HELI_CHASE_TARGET_OFFSET_WORLD_SPACE - The ped is invalid.")) {
      return;
    }

    //Ensure the vehicle chase task is valid.
    CTaskHeliChase* pTask = static_cast<CTaskHeliChase*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_HELI_CHASE));
    if (!SCRIPT_VERIFY(pTask, "SET_TASK_HELI_CHASE_TARGET_OFFSET_WORLD_SPACE - The task is invalid.")) {
      return;
    }

    //Set the target offset.
    pTask->SetTargetOffsetRelative(bTargetOffsetWorldSpace ? CTaskHeliChase::OffsetRelative_World : CTaskHeliChase::OffsetRelative_Local);
  }

  void CommandSetTaskHeliChaseOrientation(int iPedID, int iOrientationMode, int iOrientationRelative, float in_orientation) {
    //Ensure the ped is valid.
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (!SCRIPT_VERIFY(pPed, "SET_TASK_HELI_CHASE_ORIENTATION - The ped is invalid.")) {
      return;
    }

    //Ensure the vehicle chase task is valid.
    CTaskHeliChase* pTask = static_cast<CTaskHeliChase*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_HELI_CHASE));
    if (!SCRIPT_VERIFY(pTask, "SET_TASK_HELI_CHASE_ORIENTATION - The task is invalid.")) {
      return;
    }

    pTask->SetOrientationMode(static_cast<CTaskHeliChase::OrientationMode>(iOrientationMode));
    pTask->SetOrientationRelative(static_cast<CTaskHeliChase::OrientationRelative>(iOrientationRelative));
    pTask->SetOrientationOffset(in_orientation);
  }

  void CommandTaskWeaponRoll(int UNUSED_PARAM(iPedID), bool UNUSED_PARAM(bRight)) {
    Assertf(false, "unsupported command TASK_WEAPON_ROLL");
    MUST_FIX_THIS(anim
    missing);
  }

  bool CommandControlMountedWeapon(int iPedID) {
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "CONTROL_MOUNTED_WEAPON - Can't have a NULL ped")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
        if (SCRIPT_VERIFY(inVehicle, "CONTROL_MOUNTED_WEAPON: ped is not in vehicle!")) {
          if (CTaskVehicleMountedWeapon::IsTaskValid(pPed)) {
            CTask* pControlVehicleTask = pPed->GetPedIntelligence()->FindTaskPrimaryByType(CTaskTypes::TASK_CONTROL_VEHICLE);
            if (pControlVehicleTask) {
              CTaskControlVehicle* pTaskControlVehicle = static_cast<CTaskControlVehicle*>(pControlVehicleTask);
              pTaskControlVehicle->SetDesiredSubtask(NULL);
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  void CommandSetMountedWeaponTarget(int iPedID, int iOtherPedID, int iTargetVehicleID, const scrVector& scrVecCoors, int TaskMode, bool bIgnoreTargetVehDeadCheck) {
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "SET_MOUNTED_WEAPON_TARGET - Can't have a NULL ped")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        // Determine the type of target to updateSET_MOUNTED_WEAPON_TARGET
        const CEntity* pTargetEntity = NULL;

        // Ped target
        if (iOtherPedID != NULL_IN_SCRIPTING_LANGUAGE) {
          if (!SCRIPT_VERIFY(iTargetVehicleID == NULL_IN_SCRIPTING_LANGUAGE, "SET_MOUNTED_WEAPON_TARGET - Can't have a target ped and a target car"))
            return;

          const CPed* pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedID);
          if (!SCRIPT_VERIFY(pTargetPed, "SET_MOUNTED_WEAPON_TARGET - Target ped doesn't exist"))
            return;

          pTargetEntity = pTargetPed;
        }

        // Vehicle target
        if (iTargetVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
          const CVehicle* pTargetVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID, bIgnoreTargetVehDeadCheck ? CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK : CTheScripts::GUID_ASSERT_FLAGS_ALL);
          if (!SCRIPT_VERIFY(pTargetVehicle, "SET_MOUNTED_WEAPON_TARGET - Target vehicle doesn't exist"))
            return;

          pTargetEntity = pTargetVehicle;
        }

        // Set the target
        const Vector3 vTargetCoords(scrVecCoors);

        aiTask* pMountedWeaponBase = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_VEHICLE_MOUNTED_WEAPON);
        if (pMountedWeaponBase) {
          //Updated the mode
          static_cast<CTaskVehicleMountedWeapon*>(pMountedWeaponBase)->SetMode((CTaskVehicleMountedWeapon::eTaskMode) TaskMode);

          //Set the target
          if (pTargetEntity)
            static_cast<CTaskVehicleMountedWeapon*>(pMountedWeaponBase)->SetTarget(pTargetEntity, &vTargetCoords);
          else
            static_cast<CTaskVehicleMountedWeapon*>(pMountedWeaponBase)->SetTargetPosition(vTargetCoords);
        }

        Assertf(pMountedWeaponBase, "SET_MOUNTED_WEAPON_TARGET - Native function invoked but ped has no gun or vehicle gun task.");
      }
    }
  }

  void CommandClearMountedWeaponTaskUnderneathDrivingTask(int iPedID) {

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
      if (SCRIPT_VERIFY(inVehicle, "CLEAR_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK: ped is not in vehicle!")) {
        CTask* pCurrent = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CONTROL_VEHICLE);
        if (pCurrent) {
          CTaskControlVehicle* pTaskControlVehicle = static_cast<CTaskControlVehicle*>(pCurrent);
          if (scriptVerifyf(pTaskControlVehicle->GetDesiredSubtask() && pTaskControlVehicle->GetDesiredSubtask()->GetTaskType() == CTaskTypes::TASK_VEHICLE_MOUNTED_WEAPON, "CLEAR_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK no mounted weapon task running, use IS_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK")) {
            pTaskControlVehicle->SetDesiredSubtask(NULL);
          }
        }
      }
    }
  }

  bool CommandIsMountedWeaponTaskUnderneathDrivingTask(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
      if (SCRIPT_VERIFY(inVehicle, "IS_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK: ped is not in vehicle or on a mount!")) {
        CTask* pCurrent = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_VEHICLE_MOUNTED_WEAPON);
        if (pCurrent) {
          return true;
        }
      }
    }
    return false;
  }

  void CommandTaskDriveBy(int iPedID, int iOtherPedID, int iTargetVehicleID,
                          const scrVector& scrVecCoors, float fAbortRange, int iFrequencyPercentage, bool bPushUnderneathDrivingTaskIfDriving, int iFiringPatternHash) {
    Vector3 TempVec, CentreVec, VecDiff;
    CPed* pPed;
    const CPed* pSecondPed;
    const CVehicle* pSecondVehicle;
    CTask* pTask;

    TempVec = Vector3(scrVecCoors);

    if (!SCRIPT_VERIFY((iFrequencyPercentage >= 0) && (iFrequencyPercentage <= 100), "TASK_DRIVE_BY - Frequency should be between 0 and 100"))
      return;

    const CEntity* pTargetEntity = NULL;
    if (iOtherPedID != NULL_IN_SCRIPTING_LANGUAGE)  //	>= 0)
    {
      if (SCRIPT_VERIFY(iTargetVehicleID == NULL_IN_SCRIPTING_LANGUAGE, "TASK_DRIVE_BY - Can't have a target ped and a target car")) {
        pSecondPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedID);
        if (!pSecondPed)
          return;

        pTargetEntity = pSecondPed;
      } else {
        return;
      }
    } else {
      pSecondPed = NULL;
    }
    if (iTargetVehicleID != NULL_IN_SCRIPTING_LANGUAGE)  //	>= 0)
    {
      if (SCRIPT_VERIFY(iOtherPedID == NULL_IN_SCRIPTING_LANGUAGE, "TASK_DRIVE_BY - Can't have a target ped and a target car")) {
        pSecondVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID);
        if (!SCRIPT_VERIFY(pSecondVehicle, "TASK_DRIVE_BY - Target vehicle doesn't exist"))
          return;

        pTargetEntity = pSecondVehicle;
      } else {
        return;
      }
    } else {
      pSecondVehicle = NULL;
    }

    // Set up the arguments
    CAITarget target;
    if (pTargetEntity) {
      target.SetEntityAndOffset(pTargetEntity, TempVec);
    } else {
      target.SetPosition(TempVec);
    }
    const u32 uFiringPattern = iFiringPatternHash != 0 ? (u32) iFiringPatternHash : ATSTRINGHASH("FIRING_PATTERN_BURST_FIRE_DRIVEBY", 0x0d31265f2);

    bool bAlreadydoingDriveBy = false;
    if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
        bool onMount = (pPed->GetMyMount() != NULL);
        if (SCRIPT_VERIFY(inVehicle || onMount, "TASK_DRIVE_BY: char is not in vehicle or on a mount!")) {
          CTask* pControlVehicleTask = pPed->GetPedIntelligence()->FindTaskPrimaryByType(CTaskTypes::TASK_CONTROL_VEHICLE);
          CTask* pCurrent = pPed->GetPedIntelligence()->GetTaskAtPriority(PED_TASK_PRIORITY_PRIMARY);
          if (pCurrent && pCurrent->GetTaskType() == CTaskTypes::TASK_VEHICLE_GUN) {
            CTaskVehicleGun* pTaskDriveBy = (CTaskVehicleGun*) pCurrent;
            if (pTaskDriveBy->GetTarget().GetEntity() == pSecondPed) {
              bAlreadydoingDriveBy = true;
            }
          } else if (bPushUnderneathDrivingTaskIfDriving && pControlVehicleTask) {
            CTaskControlVehicle* pTaskControlVehicle = static_cast<CTaskControlVehicle*>(pControlVehicleTask);
            pTask = rage_new
            CTaskVehicleGun(CTaskVehicleGun::Mode_Fire, uFiringPattern, &target, ((float) iFrequencyPercentage) / 100.0f, fAbortRange);
            pTaskControlVehicle->SetDesiredSubtask(pTask);
            bAlreadydoingDriveBy = true;
          } else if (onMount && bPushUnderneathDrivingTaskIfDriving) {
            CTaskComplexControlMovement* pControlTask = static_cast<CTaskComplexControlMovement*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT));
            if (pControlTask) {
              const int existingSubTask = pControlTask->GetMainSubTaskType();
              if (existingSubTask == CTaskTypes::TASK_NONE || existingSubTask == CTaskTypes::TASK_VEHICLE_GUN) {
                pTask = rage_new
                CTaskVehicleGun(CTaskVehicleGun::Mode_Fire, uFiringPattern, &target, ((float) iFrequencyPercentage) / 100.0f, fAbortRange);
                pControlTask->SetNewMainSubtask(pTask);
                bAlreadydoingDriveBy = true;
              }
              // Note: not entirely sure what to do in the case of there being a subtask that's something
              // else than CTaskVehicleGun. Now, it would still give you the driveby task, but running as the
              // main task. This seems consistent with the vehicle case and is probably acceptable.
            }
          }
        }
      }
    }

    if (!bAlreadydoingDriveBy) {
      pTask = rage_new
      CTaskVehicleGun(CTaskVehicleGun::Mode_Fire, uFiringPattern, &target, ((float) iFrequencyPercentage) / 100.0f, fAbortRange);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_DRIVE_BY, "TASK_DRIVE_BY");
    }
  }

  void CommandSetDriveByTarget(int iPedID, int iOtherPedID, int iTargetVehicleID, const scrVector& scrVecCoors) {
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "SET_DRIVERBY_TASK_TARGET - Can't have a NULL ped")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        // Determine the type of target to update
        const CEntity* pTargetEntity = NULL;

        // Ped target
        if (iOtherPedID != NULL_IN_SCRIPTING_LANGUAGE) {
          if (!SCRIPT_VERIFY(iTargetVehicleID == NULL_IN_SCRIPTING_LANGUAGE, "SET_DRIVERBY_TASK_TARGET - Can't have a target ped and a target car"))
            return;

          const CPed* pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedID);
          if (!SCRIPT_VERIFY(pTargetPed, "SET_DRIVERBY_TASK_TARGET - Target ped doesn't exist"))
            return;

          pTargetEntity = pTargetPed;
        }

        // Vehicle target
        if (iTargetVehicleID != NULL_IN_SCRIPTING_LANGUAGE) {
          const CVehicle* pTargetVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleID);
          if (!SCRIPT_VERIFY(pTargetVehicle, "SET_DRIVERBY_TASK_TARGET - Target vehicle doesn't exist"))
            return;

          pTargetEntity = pTargetVehicle;
        }

        // Set the target
        const Vector3 vTargetCoords(scrVecCoors);

        // Check the gun task
        aiTask* pGunTaskBase = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_GUN);
        if (pGunTaskBase) {
          CWeaponTarget target;
          if (pTargetEntity)
            target.SetEntityAndOffset(pTargetEntity, vTargetCoords);
          else
            target.SetPosition(vTargetCoords);

          static_cast<CTaskGun*>(pGunTaskBase)->SetTarget(target);
        }

        // Check vehicle gun task just in case
        aiTask* pVehicleGunTaskBase = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_VEHICLE_GUN);
        if (pVehicleGunTaskBase) {
          if (pTargetEntity)
            static_cast<CTaskVehicleGun*>(pVehicleGunTaskBase)->SetTarget(pTargetEntity, &vTargetCoords);
          else
            static_cast<CTaskVehicleGun*>(pVehicleGunTaskBase)->SetTargetPosition(vTargetCoords);
        }

        Assertf(pGunTaskBase || pVehicleGunTaskBase, "SET_DRIVERBY_TASK_TARGET - Native function invoked but ped has no gun or vehicle gun task.");
      }
    }
  }

  void CommandClearDrivebyTaskUnderneathDrivingTask(int iPedID) {

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
      bool onMount = (pPed->GetMyMount() != NULL);
      if (SCRIPT_VERIFY(inVehicle || onMount, "CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK: char is not in vehicle or on a mount!")) {
        CTask* pCurrent = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CONTROL_VEHICLE);
        if (pCurrent) {
          CTaskControlVehicle* pTaskControlVehicle = static_cast<CTaskControlVehicle*>(pCurrent);
          if (scriptVerifyf(pTaskControlVehicle->GetDesiredSubtask() && pTaskControlVehicle->GetDesiredSubtask()->GetTaskType() == CTaskTypes::TASK_VEHICLE_GUN, "CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK no driveby task running, use IS_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK")) {
            pTaskControlVehicle->SetDesiredSubtask(NULL);
          }
        } else if (onMount) {
          CTaskComplexControlMovement* pControlTask = static_cast<CTaskComplexControlMovement*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT));
          if (pControlTask) {
            const int existingSubTask = pControlTask->GetMainSubTaskType();
            if (existingSubTask == CTaskTypes::TASK_VEHICLE_GUN) {
              pControlTask->SetNewMainSubtask(NULL);
            }
          }
        }
      }
    }
  }

  bool CommandIsDrivebyTaskUnderneathDrivingTask(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      bool inVehicle = pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle);
      bool onMount = (pPed->GetMyMount() != NULL);
      if (SCRIPT_VERIFY(inVehicle || onMount, "IS_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK: char is not in vehicle or on a mount!")) {
        CTask* pCurrent = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CONTROL_VEHICLE);
        if (pCurrent) {
          CTaskControlVehicle* pTaskControlVehicle = static_cast<CTaskControlVehicle*>(pCurrent);
          return (pTaskControlVehicle->GetDesiredSubtask() && pTaskControlVehicle->GetDesiredSubtask()->GetTaskType() == CTaskTypes::TASK_VEHICLE_GUN);
        } else if (onMount) {
          CTaskComplexControlMovement* pControlTask = static_cast<CTaskComplexControlMovement*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT));
          if (pControlTask) {
            const int existingSubTask = pControlTask->GetMainSubTaskType();
            return existingSubTask == CTaskTypes::TASK_VEHICLE_GUN;
          }
        }
      }
    }
    return false;
  }

  void CommandTaskUseMobilePhone(int iPedID, bool bUsePhone, int eDesiredPhoneMode) {
    if (bUsePhone) {
      if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
        // pulled inside due to the NULL check
        bool bShouldCreatePhoneTask = true;
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
        if (pPed) {
          CTask* pTask = (bShouldCreatePhoneTask ? pPed->GetPedIntelligence()->FindTaskActiveByTreeAndType(PED_TASK_TREE_SECONDARY, CTaskTypes::TASK_MOBILE_PHONE) : NULL);
          if (pTask) {
            CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pTask);
            pTaskMobilePhone->PutUpToEar();
            bShouldCreatePhoneTask = false;
          }

          pTask = (bShouldCreatePhoneTask ? pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_MOTION_IN_COVER) : NULL);
          if (pTask && pTask->GetTaskType() == CTaskTypes::TASK_MOTION_IN_COVER) {
            CTaskMotionInCover* pTaskCover = (CTaskMotionInCover*) pTask;
            pTaskCover->ProcessUseMobilePhone(true);  // Pick up phone
            bShouldCreatePhoneTask = false;
          }
        }
        if (bShouldCreatePhoneTask) {
          CTask* pTask = rage_new
          CTaskMobilePhone(CTaskMobilePhone::PhoneMode(eDesiredPhoneMode));
          pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
        }
      } else {
        // We still have to try and send the task if this is called from script in a sequence.
        CTask* pTask = rage_new
        CTaskMobilePhone(CTaskMobilePhone::PhoneMode(eDesiredPhoneMode), -1, false);
        CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_USE_MOBILE_PHONE, "TASK_USE_MOBILE_PHONE");
      }
    } else {
      if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "TASK_USE_MOBILE_PHONE( ped, FALSE ) Cannot be used in a sequence")) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
        if (pPed) {
          CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByTreeAndType(PED_TASK_TREE_SECONDARY, CTaskTypes::TASK_MOBILE_PHONE);
          if (pTask) {
            CTaskMobilePhone* pTaskPhone = (CTaskMobilePhone*) pTask;
            pTaskPhone->PutDownToPrevState();
          }

          // Make sure any mobile phone tasks in the event queue are removed.
          for (s32 i = 0; i < pPed->GetPedIntelligence()->GetNumEventsInQueue(); i++) {
            CEvent* pEvent = pPed->GetPedIntelligence()->GetEventByIndex(i);
            if (pEvent && pEvent->GetEventType() == EVENT_SCRIPT_COMMAND) {
              CEventScriptCommand* pEventScript = static_cast<CEventScriptCommand*>(pEvent);
              if (pEventScript->GetTask() && pEventScript->GetTask()->GetTaskType() == CTaskTypes::TASK_MOBILE_PHONE) {
                pPed->GetPedIntelligence()->RemoveEvent(pEvent);
              }
            }
          }
        }
      }
    }
  }

  void CommandAddFollowNavmeshToPhoneTask(int iPedID, const scrVector& svTargetPos, float fMoveBlendRatio) {
    Assert(fMoveBlendRatio >= MOVEBLENDRATIO_WALK && fMoveBlendRatio <= MOVEBLENDRATIO_SPRINT);

    Vector3 vTargetPos = svTargetPos;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedID, "ADD_FOLLOW_NAVMESH_TO_PHONE_TASK Cannot be used in a sequence")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        CTask* pTaskSecondary = pPed->GetPedIntelligence()->FindTaskActiveByTreeAndType(PED_TASK_TREE_SECONDARY, CTaskTypes::TASK_MOBILE_PHONE);
        if (pTaskSecondary) {
          CTaskComplexControlMovement* pControlTask = static_cast<CTaskComplexControlMovement*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT));
          if (pControlTask) {
            CTaskMoveFollowNavMesh* pNavMeshTask = rage_new
            CTaskMoveFollowNavMesh(fMoveBlendRatio, vTargetPos);
            pNavMeshTask->SetIsScriptedRoute(true);
            pControlTask->SetNewMoveTask(pNavMeshTask);
          }
        }
      }
    }
  }

  void CommandTaskUseMobilePhoneTimed(int iPedID, int iTime) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_USE_MOBILE_PHONE_TIMED - Time must be -1 for infinite and > 0")) {
      if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);

        CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByTreeAndType(PED_TASK_TREE_SECONDARY, CTaskTypes::TASK_MOBILE_PHONE);
        if (!pTask) {
          pTask = rage_new
          CTaskMobilePhone(CTaskMobilePhone::Mode_ToCall, iTime);
          pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
        } else {
          // we already have one, just reset it
          CTaskMobilePhone* pTaskMobilePhone = static_cast<CTaskMobilePhone*>(pTask);
          pTaskMobilePhone->ResetTaskMobilePhone(CTaskMobilePhone::Mode_ToCall, iTime);
        }
      } else {
        // We still have to try and send the task if this is called from script in a sequence.
        CTask* pTask = rage_new
        CTaskMobilePhone(CTaskMobilePhone::Mode_ToCall, iTime, false);
        CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_USE_MOBILE_PHONE, "TASK_USE_MOBILE_PHONE_TIMED");
      }
    }
  }

  void CommandTaskUseWalkieTalkie(int UNUSED_PARAM(iPedID), const char* UNUSED_PARAM(szContext)) {
    scriptAssertf(0, "TASK_USE_WALKIE_TALKIE has been deprecated");
  }

  void CommandTaskCapturePed(int UNUSED_PARAM(iPedID), int UNUSED_PARAM(iOtherPedID), float UNUSED_PARAM(fCaptureTime), float UNUSED_PARAM(fTimeToQuestion)) {
    scriptAssertf(0, "TASK_CAPTURE_PED is deprecated");
  }

  void CommandTaskAdvanceToTargetInLine(int iPedID, int iOtherPedID, const scrVector& svTargetLocation, float fDesiredDist, float fTimeToHoldAtEnd, int iFlags) {
    // If we aren't running this in a sequence, assert on the peds group
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      Assertf(pPed->GetPedsGroup(), "Ped running TASK_ADVANCE_TO_TARGET_IN_LINE must have a valid group");
    }

    // Get the other ped or location or both to determine the target type
    CPed* pOtherPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iOtherPedID);
    Vector3 vTargetLoc = svTargetLocation;

    Assertf(pOtherPed || vTargetLoc.IsNonZero(), "Target must be a valid ped or a non-origin position");

    CAITarget target;
    if (pOtherPed) {
      target = vTargetLoc.IsZero() ? CAITarget(pOtherPed) : CAITarget(pOtherPed, vTargetLoc);
    } else {
      target = CAITarget(vTargetLoc);
    }

    // Create our new task and give it to the ped
    CTask* pTask = rage_new
    CTaskSwatFollowInLine(target, fDesiredDist, fTimeToHoldAtEnd, iFlags);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_ADVANCE_TO_TARGET_IN_LINE, "TASK_ADVANCE_TO_TARGET_IN_LINE");
  }

  void CommandSetChargeTargetEnabled(bool bEnabled) {
    CTaskCombat::ms_Tunables.m_ChargeTuning.m_bChargeTargetEnabled = bEnabled;
  }

  void CommandSetMaxNumActiveChargers(int iMaxActiveChargers) {
    if (SCRIPT_VERIFY(iMaxActiveChargers >= 0, "SET_MAX_NUM_ACTIVE_CHARGERS -- negative MAX_NUM_ACTIVE_CHARGERS requested")) {
      CTaskCombat::ms_Tunables.m_ChargeTuning.m_uMaxNumActiveChargers = (u8) iMaxActiveChargers;
    }
  }

  void CommandSetMinTimeBetweenChargesAtSameTarget(float fMinTimeSeconds) {
    if (SCRIPT_VERIFY(fMinTimeSeconds >= 0.0f, "SET_MIN_TIME_BETWEEN_CHARGES_AT_SAME_TARGET -- negative MIN_TIME_SECONDS requested")) {
      CTaskCombat::ms_Tunables.m_ChargeTuning.m_uMinTimeBetweenChargesAtSameTargetMS = (u32)(fMinTimeSeconds * 1000.0f);
    }
  }

  void CommandTaskChatToPed(int iPedID, int iOtherPedID, int iFlags, const scrVector& vGoToPos, float fHeadingDegs, float fIdleTime) {
    CPed* pOtherPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iOtherPedID);
    if (pOtherPed) {
      // Convert the heading to radians, in the -PI..PI range.
      float fHeading = CanonicalizeAngle(fHeadingDegs * DtoR);

      CTask* pTask = rage_new
      CTaskChat(pOtherPed, iFlags, vGoToPos, fHeading, fIdleTime);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_CHAT_TO_PED, "TASK_CHAT_TO_PED");
    }
  }

  bool CommandIsChattingPedInPosition(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);

    if (pPed) {
      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CHAT);
      if (pTask && static_cast<CTaskChat*>(pTask)->IsInPosition()) {
        return true;
      }
    }
    return false;
  }

  bool CommandIsChattingPedPlayingAnim(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);

    if (pPed) {
      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CHAT);
      if (pTask && static_cast<CTaskChat*>(pTask)->IsPlayingClip()) {
        return true;
      }
    }
    return false;
  }

  void CommandMakeChattingPedPlayAnim(int UNUSED_PARAM(iPedID), const char* UNUSED_PARAM(pAnimDictName), const char* UNUSED_PARAM(pAnimName), float UNUSED_PARAM(fBlendInDelta), int UNUSED_PARAM(nTimeToPlay), int UNUSED_PARAM(nControlFlags)) {
    // GSALES - killing this so I can get rid of the old anim control flag stuff. It doesn't seem to be used anywhere any more.
    scriptAssertf(0, "MAKE_CHATTING_PED_PLAY_ANIM - This command is deprecated. See an animation programmer.");
  }

  void CommandMakeChattingPedLeave(int iPedID, bool bPlayGoodByeGestures) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);

    if (pPed) {
      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CHAT);
      if (pTask) {
        static_cast<CTaskChat*>(pTask)->LeaveChat(bPlayGoodByeGestures);
      }
    }
  }

  enum {
    MOBILE_SUB_TASK_PHONE_IN,
    MOBILE_SUB_TASK_PHONE_TALK,
    MOBILE_SUB_TASK_PHONE_OUT,
    MOBILE_SUB_TASK_PHONE_OTHER
  };

  bool CommandGetMobilePhoneTaskSubTask(int iPedID, int& ReturnSubTask) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed) {
      ReturnSubTask = MOBILE_SUB_TASK_PHONE_IN;
      CTaskComplexUseMobilePhone* pTaskUseMobile = (CTaskComplexUseMobilePhone*) pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_COMPLEX_USE_MOBILE_PHONE);

      //	Assertf(pTaskUseMobile, "%s:GET_MOBILE_PHONE_TASK_SUB_TASK - ped is not performing the mobile phone task", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      if (pTaskUseMobile) {
        CTask* pSubTask = pTaskUseMobile->GetSubTask();

        if (pSubTask) {
          switch (pSubTask->GetTaskType()) {
            case CTaskTypes::TASK_SIMPLE_PHONE_IN:
              ReturnSubTask = MOBILE_SUB_TASK_PHONE_IN;
              break;
            case CTaskTypes::TASK_SIMPLE_PHONE_TALK:
              ReturnSubTask = MOBILE_SUB_TASK_PHONE_TALK;
              break;
            case CTaskTypes::TASK_SIMPLE_PHONE_OUT:
              ReturnSubTask = MOBILE_SUB_TASK_PHONE_OUT;
              break;
            default:  //	return the same value for all other sub task types?
              ReturnSubTask = MOBILE_SUB_TASK_PHONE_OTHER;
              break;
          }
        }

        return true;
      }

      //	if the ped isn't performing TASK_COMPLEX_USE_MOBILE_PHONE then ReturnSubTask will be MOBILE_SUB_TASK_PHONE_IN
    }
    return false;
  }

  void CommandTaskWarpPedIntoVehicle(int iPedID, int iVehicleID, int iSeat) {
    scriptAssertf(!NetworkInterface::IsGameInProgress(), "%s:TASK_WARP_PED_INTO_VEHICLE is not safe in network games! Use TASK_ENTER_VEHICLE with the warp flag set instead!", CTheScripts::GetCurrentScriptNameAndProgramCounter());

    CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);

    if (pVehicle) {
      s32 iSeatIndex = iSeat + 1;

      if (iSeatIndex == -1) {
        // If -2 is passed in, we should put the ped into any passenger seat
        for (s32 seatIndex = 1; seatIndex < pVehicle->GetSeatManager()->GetMaxSeats(); seatIndex++) {
          if (!pVehicle->GetSeatManager()->GetPedInSeat(seatIndex)) {
            iSeatIndex = seatIndex;
            break;
          }
        }

        if (!SCRIPT_VERIFY(iSeatIndex != -1, "TASK_WARP_PED_INTO_VEHICLE - Couldn't find free passenger seat to put ped in"))
          return;
      }

      if (iSeatIndex != -1) {
        if (!SCRIPT_VERIFY((iSeatIndex < pVehicle->GetSeatManager()->GetMaxSeats()), "TASK_WARP_PED_INTO_VEHICLE - Seat number is too large"))
          return;

        if (!SCRIPT_VERIFY(!pVehicle->GetSeatManager()->GetPedInSeat(iSeatIndex), "TASK_WARP_PED_INTO_VEHICLE - Vehicle already has a passenger in the specified seat"))
          return;
      }

      CTaskSetPedInVehicle* pTask = rage_new
      CTaskSetPedInVehicle(pVehicle, iSeatIndex, CPed::PVF_Warp);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_WARP_PED_INTO_VEHICLE, "TASK_WARP_PED_INTO_VEHICLE");
    }
  }

  // Warps a ped to a position on an entity with defined combination of limbs attached and sets the ped to ragdoll.
  void CommandTaskNMAttachPedToEntity(int iPedID, int iEntityID, int iAttachParentBone, const scrVector& vOffset, const scrVector& vRotation, int iAttachFlags, const char* pAnimDict, const char* pGetOnAnimName, const char* pIdleAnimName, int RotOrder) {
    s32 animDictIndex;

    CPhysical* pEntity = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(iEntityID);

    if (scriptVerifyf(!NetworkInterface::IsGameInProgress(), "TASK_NM_ATTACH_PED_TO_ENTITY - This script command is not allowed in network game scripts!")) {
      if (pEntity) {
        if (scriptVerifyf(fwAnimManager::GetClipIfExistsByName(pAnimDict, pGetOnAnimName), "Check that the dictionary: %s is loaded and the anim: %s is contained with the dictionary", pAnimDict, pGetOnAnimName)) {
          if (scriptVerifyf(fwAnimManager::GetClipIfExistsByName(pAnimDict, pIdleAnimName), "Check that the dictionary: %s is loaded and the anim: %s is contained with the dictionary", pAnimDict, pIdleAnimName)) {
            animDictIndex = fwAnimManager::FindSlot(pAnimDict).Get();

            Quaternion rotationQuaternion;
            CScriptEulers::QuaternionFromEulers(rotationQuaternion, vRotation, static_cast<EulerAngleOrder>(RotOrder));

            CTaskAnimatedAttach* pTaskAttach = rage_new
            CTaskAnimatedAttach(pEntity, iAttachParentBone, vOffset, rotationQuaternion, iAttachFlags, animDictIndex, atStringHash(pGetOnAnimName), atStringHash(pIdleAnimName));
            CScriptPeds::GivePedScriptedTask(iPedID, pTaskAttach, SCRIPT_TASK_NM_ATTACH_TO_VEHICLE, "TASK_NM_ATTACH_PED_TO_ENTITY");
          }
        }
      }
    }
  }

  void CommandTaskShootAtEntity(int iPedID, int iEntityID, int iTime, int iFiringPatternHash) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SHOOT_AT_ENTITY - Time must be -1 for infinite and > 0")) {
      const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
      if (pEntity) {
        CTaskGun* pTaskGun = rage_new
        CTaskGun(CWeaponController::WCT_Fire, CTaskTypes::TASK_AIM_GUN_ON_FOOT, CWeaponTarget(pEntity), ((float) iTime) / 1000.0f);
        pTaskGun->SetAllowRestartAfterAbort(true);
        if (iFiringPatternHash) {
          pTaskGun->SetFiringPatternHash(iFiringPatternHash);
        }
        CScriptPeds::GivePedScriptedTask(iPedID, pTaskGun, SCRIPT_TASK_SHOOT_AT_ENTITY, "TASK_SHOOT_AT_ENTITY");

        #if 0
                                                                                                                                // This block of code is an attempt to make this task work for a mounted ped, left in
				// for reference. Was hoping that we could just use CTaskGun with CTaskAimGunVehicleDriveBy,
				// but there were some complications. Basically, if we bypass CTaskVehicleGun, there is nothing
				// streaming in the animations that CTaskAimGunVehicleDriveBy needs. If on the other hand
				// we create a CTaskVehicleGun instead of CTaskGun, we probably won't have support for
				// the time parameter. /FF

				CTaskTypes::eTaskType aimTaskType = CTaskTypes::TASK_AIM_GUN_ON_FOOT;
				const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
				if(pPed && pPed->GetMyMount())
				{
					aimTaskType = CTaskTypes::TASK_AIM_GUN_VEHICLE_DRIVE_BY;
				}

				CTaskGun* pTaskGun = rage_new CTaskGun(CWeaponController::WCT_Fire, aimTaskType, CWeaponTarget(pEntity), ((float) iTime) / 1000.0f);

				if(aimTaskType == CTaskTypes::TASK_AIM_GUN_VEHICLE_DRIVE_BY)
				{
					const CVehicleDriveByInfo* pDrivebyInfo = CVehicleMetadataMgr::GetVehicleDriveByInfoFromPed(pPed);
					taskAssert(pDrivebyInfo);
					const CVehicleDriveByAnimInfo* pAnimInfo = CVehicleMetadataMgr::GetDriveByAnimInfoForWeapon(pPed, pPed->GetWeaponManager()->GetEquippedWeaponHash());
					pTaskGun->SetOverrideClipSetId(pAnimInfo->GetClipSet());
				}
        #endif
      }
    }
  }

  void CommandTaskClimb(int iPedID, bool UNUSED_PARAM(bUsePlayerLaunchForce)) {
    CTask* pTask = rage_new
    CTaskJumpVault(JF_ForceVault);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_CLIMB, "TASK_CLIMB");
  }

  void CommandTaskClimbLadder(int iPedID, bool bFast) {
    CTask* pTask = rage_new
    CTaskClimbLadderFully(bFast);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_CLIMB_LADDER, "TASK_CLIMB_LADDER");
  }

  void CommandTaskRappelDownWall(int iPedID, const scrVector& scrVecStartPos, const scrVector& scrVecRopeAnchorPos, float fDestinationHeight, int ropeID) {
    ropeManager* pRopeManager = CPhysics::GetRopeManager();
    if (SCRIPT_VERIFY(pRopeManager, "Rope Manager doesn't exist or is damaged!")) {
      ropeInstance* pRope = pRopeManager->FindRope(ropeID);
      if (scriptVerifyf(pRope, "Rope with ID: %d, doesn't exist or is damaged!", ropeID)) {
        Vector3 vStartPos(scrVecStartPos);
        Vector3 vAnchorPos(scrVecRopeAnchorPos);
        CTask* pTask = rage_new
        CTaskRappel(fDestinationHeight, vStartPos, vAnchorPos, ropeID, CLIP_SET_ID_INVALID);
        CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_RAPPEL_DOWN_WALL, "TASK_RAPPEL_DOWN_WALL");
      }
    }
  }

  void CommandTaskRappelDownWallUsingClipsetOverride(int iPedID, const scrVector& scrVecStartPos, const scrVector& scrVecRopeAnchorPos, float fDestinationHeight, int ropeID, const char* szOverrideRappelClipset, bool bSkipClimbOverWall) {
    ropeManager* pRopeManager = CPhysics::GetRopeManager();
    if (SCRIPT_VERIFY(pRopeManager, "Rope Manager doesn't exist or is damaged!")) {
      ropeInstance* pRope = pRopeManager->FindRope(ropeID);
      if (scriptVerifyf(pRope, "Rope with ID: %d, doesn't exist or is damaged!", ropeID)) {
        if (szOverrideRappelClipset) {
          fwMvClipSetId setId(szOverrideRappelClipset);
          if (SCRIPT_VERIFYF(fwClipSetManager::GetClipSet(setId) != NULL, "ClipSet %s did not exist.  Are you sure it is in clip_sets.xml?", szOverrideRappelClipset)) {
            Vector3 vStartPos(scrVecStartPos);
            Vector3 vAnchorPos(scrVecRopeAnchorPos);
            CTask* pTask = rage_new
            CTaskRappel(fDestinationHeight, vStartPos, vAnchorPos, ropeID, setId, bSkipClimbOverWall);
            CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_RAPPEL_DOWN_WALL, "TASK_RAPPEL_DOWN_WALL");
          }
        }
      }
    }
  }

  int CommandsGetTaskRappelDownWallState(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (pPed && pPed->GetPedIntelligence()->GetQueriableInterface()->IsTaskCurrentlyRunning(CTaskTypes::TASK_RAPPEL)) {
      return pPed->GetPedIntelligence()->GetQueriableInterface()->GetStateForTaskType(CTaskTypes::TASK_RAPPEL);
    }

    return -1;
  }

  void CommandClearPedTasksImmediately(int iPedID) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);

    if (pPed) {
      pPed->SetPedResetFlag(CPED_RESET_FLAG_ScriptClearingPedTasks, true);
      if (pPed->IsNetworkClone()) {
        bool bClearTasksImmediately = true;
        CClearPedTasksEvent::Trigger(pPed, bClearTasksImmediately);
      } else {
        AI_LOG_WITH_ARGS_IF_SCRIPT_OR_PLAYER_PED(pPed, "[Script] - Ped %s has had tasks cleared immediately by script %s\n", AILogging::GetDynamicEntityNameSafe(pPed), CTheScripts::GetCurrentScriptName());

        // instantly blend out any remaining animation before the flush
        pPed->GetMovePed().SwitchToAnimated(0, false, true);

        pPed->GetPedIntelligence()->FlushImmediately(true);
        pPed->SetIsCrouching(false);
        pPed->StopAllMotion(true);

        // B*2979918 - too late to change for SP, but don't clear the helmet in MP when clearing tasks. It should be managed separately
        if (!NetworkInterface::IsGameInProgress()) {
          if (pPed->GetHelmetComponent())
            pPed->GetHelmetComponent()->DisableHelmet();
        }

        pPed->SetPedResetFlag(CPED_RESET_FLAG_IsDrowning, false);
      }
      pPed->SetTimeSincePedInWater(60.0f); //B* 1150689

      #if __DEV
                                                                                                                              // Record the task in the debug list
			static char stringName[] = "CLEAR_PED_TASKS_IMMEDIATELY";
			pPed->GetPedIntelligence()->AddScriptHistoryString(stringName, stringName, CTheScripts::GetCurrentScriptName(), CTheScripts::GetCurrentGtaScriptThread()->GetThreadPC());
      #endif
    }
  }

  void CommandTaskPerformSequenceFromProgress(int iPedID, int iTaskIndex, int iProgress1, int iProgress2) {
    const int iTaskID = GetActualSequenceId(iTaskIndex);
    if (SCRIPT_VERIFY(iTaskID >= 0 && iTaskID < CTaskSequences::MAX_NUM_SEQUENCE_TASKS, "TASK_PERFORM_SEQUENCE_FROM_PROGRESS - Sequence task ID too big")) {
      if (SCRIPT_VERIFY(!CTaskSequences::ms_bIsOpened[iTaskID], "TASK_PERFORM_SEQUENCE_FROM_PROGRESS - Sequence task is still open")) {
        if (SCRIPT_VERIFY(!CTaskSequences::ms_TaskSequenceLists[iTaskID].IsEmpty(), "TASK_PERFORM_SEQUENCE_FROM_PROGRESS - sequence task empty")) {
          if (SCRIPT_VERIFY(iProgress1 >= 0 && iProgress1 < CTaskList::MAX_LIST_SIZE, "TASK_PERFORM_SEQUENCE_FROM_PROGRESS - Sequence task ID too big")) {
            if (SCRIPT_VERIFY(iProgress2 < CTaskList::MAX_LIST_SIZE, "TASK_PERFORM_SEQUENCE_FROM_PROGRESS - Sequence task ID too big")) {
              CTaskUseSequence* pTask = rage_new
              CTaskUseSequence(iTaskID, iTaskIndex);
              pTask->SetProgress((u32) iProgress1, iProgress2);
              CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PERFORM_SEQUENCE_FROM_PROGRESS, "TASK_PERFORM_SEQUENCE_FROM_PROGRESS");
            }
          }
        }
      }
    }
  }

  float CommandGetPedDesiredMoveBlendRatio(int iPedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);

    if (pPed) {
      return pPed->GetMotionData()->GetDesiredMbrY();
    }
    return 0;
  }

  void CommandSetNextDesiredMoveState(float /*NextMoveState*/) {
    scriptAssertf(0, "%s:SET_NEXT_DESIRED_MOVE_STATE - Function is deprecated - do not use anymore", CTheScripts::GetCurrentScriptNameAndProgramCounter());
  }

  void CommandSetPedDesiredMoveBlendRatio(int iPedID, float fNewMoveBlendRatio) {
    if (!scriptVerifyf(fNewMoveBlendRatio >= MOVEBLENDRATIO_STILL && fNewMoveBlendRatio <= MOVEBLENDRATIO_SPRINT, "SET_PED_DESIRED_MOVE_BLEND_RATIO - move blend ratio %f out of range (%.2f:%.2f)", fNewMoveBlendRatio, MOVEBLENDRATIO_STILL, MOVEBLENDRATIO_SPRINT)) {
      fNewMoveBlendRatio = Clamp(fNewMoveBlendRatio, MOVEBLENDRATIO_STILL, MOVEBLENDRATIO_SPRINT);
    }

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);

    if (pPed) {
      //Assertf(fMoveBlendRatio >= MOVEBLENDRATIO_STILL && fMoveBlendRatio <= MOVEBLENDRATIO_SPRINT, "MODIFY_PED_MOVE_BLEND_RATIO - Ratio must be between 0..3 inclusive");

      // Generally speaking we will want to look for the control-movement task in the primary (script) slot.
      // However there's an exception to this rule : if the ped is in a group, they may be entering a vehicle
      // via a task which has been given them in response to CEventLeaderEnteredCarAsDriver (etc), in which
      // case we will want to find the control movement in the TASK_PRIORITY_EVENT_RESPONSE_NONTEMP slot.
      CTaskComplexControlMovement* pCtrlMove = NULL;

      CPedIntelligence* pPedAi = pPed->GetPedIntelligence();
      const int iTaskPriority = pPedAi->GetTaskManager()->GetActiveTaskPriority(PED_TASK_TREE_PRIMARY);

      if (pPed->GetPedsGroup() != NULL && iTaskPriority == PED_TASK_PRIORITY_EVENT_RESPONSE_NONTEMP
          && pPedAi->GetCurrentEventType() == EVENT_LEADER_ENTERED_CAR_AS_DRIVER) {
        pCtrlMove = (CTaskComplexControlMovement*) pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT);
      } else {
        pCtrlMove = (CTaskComplexControlMovement*) pPed->GetPedIntelligence()->FindTaskPrimaryByType(CTaskTypes::TASK_COMPLEX_CONTROL_MOVEMENT);
      }
      if (!pCtrlMove) {
        return;
      }

      CTask* pRunningMoveTask = pCtrlMove->GetRunningMovementTask(pPed);
      CTask* pBackUpMoveTask = pCtrlMove->GetBackupCopyOfMovementSubtask();

      if (pRunningMoveTask) {
        Assert(pRunningMoveTask->IsMoveTask());
        pRunningMoveTask->PropagateNewMoveSpeed(fNewMoveBlendRatio);
      }
      if (pBackUpMoveTask) {
        Assert(pBackUpMoveTask->IsMoveTask());
        pBackUpMoveTask->PropagateNewMoveSpeed(fNewMoveBlendRatio);
      }
    }
  }

  void CommandTaskGotoEntityAiming(int iPedID, int iEntityID, float fSeekRadius, float fAimRadius) {
    CTask* pTask;
    const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(iEntityID);
    if (pEntity) {
      Assert(fAimRadius >= fSeekRadius);
      pTask = rage_new
      CTaskSeekEntityAiming(pEntity, fSeekRadius, fAimRadius);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_GOTO_ENTITY_AIMING, "TASK_GOTO_ENTITY_AIMING");
    }
  }

  void CommandTaskSetDecisionMaker(int iPedID, int iDecisionMakerId) {
    if (SCRIPT_VERIFY(iDecisionMakerId != NULL_IN_SCRIPTING_LANGUAGE, "TASK_SET_DECISION_MAKER -  NULL passed for decision maker, or variable not initialised")) {
      if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
        CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
        if (pPed) {
          if (SCRIPT_VERIFY(pPed->IsPlayer() == false, "TASK_SET_PED_DECISION_MAKER - Can't change a player's decision maker")) {
            pPed->GetPedIntelligence()->SetDecisionMakerId(iDecisionMakerId);
          }
        }
      } else {
        CTask* pTask = rage_new
        CTaskSetCharDecisionMaker(iDecisionMakerId);
        CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SET_PED_DECISION_MAKER, "TASK_SET_DECISION_MAKER");
      }
    }
  }

  void SetPedDefensiveIfNeeded(CPed* pPed) {
    if (pPed) {
      // If our ped is set as will advance movement then change it to defensive
      CCombatBehaviour& pedCombatBehaviour = pPed->GetPedIntelligence()->GetCombatBehaviour();
      if (pedCombatBehaviour.GetCombatMovement() == CCombatData::CM_WillAdvance) {
        pedCombatBehaviour.SetCombatMovement(CCombatData::CM_Defensive);
      }
    }
  }

  void CommandTaskSetSphereDefensiveArea(int iPedID, const scrVector& scrVecCenter, float fRadius) {
    Vector3 vCenter(scrVecCenter);

    if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed) {
        pPed->GetPedIntelligence()->GetDefensiveAreaManager()->GetPrimaryDefensiveArea()->SetAsSphere(vCenter, fRadius, NULL);
        SetPedDefensiveIfNeeded(pPed);
      }
    } else {
      CTask* pTask = rage_new
      CTaskSetPedDefensiveArea(vCenter, fRadius);
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SET_PED_DEFENSIVE_AREA, "TASK_SET_PED_SPHERE_DEFENSIVE_AREA");
    }
  }

  void CommandTaskClearDefensiveArea(int iPedID) {
    bool bGivePedTask = true;

    if (NULL_IN_SCRIPTING_LANGUAGE != iPedID) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (pPed && !pPed->IsNetworkClone()) {
        pPed->GetPedIntelligence()->GetDefensiveAreaManager()->GetPrimaryDefensiveArea()->Reset();
        bGivePedTask = false;
      }
    }

    if (bGivePedTask) {
      CTask* pTask = rage_new
      CTaskSetPedDefensiveArea();
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_SET_PED_DEFENSIVE_AREA, "TASK_CLEAR_DEFENSIVE_AREA");
    }
  }

  void CommandTaskPedSlideToCoordWithHeadingChangeRate(int iPedID, const scrVector& scrVecCoors, float fHeadingDegs, float fSpeed, float fHdgChangeRate) {
    while (fHeadingDegs < 0.0f)
      fHeadingDegs += 360.0f;
    while (fHeadingDegs >= 360.0f)
      fHeadingDegs -= 360.0f;
    const float heading = (DtoR * fHeadingDegs);
    float speed = fSpeed;
    if (speed < 0) {
      speed = 0.1f;
    }
    Vector3 v(scrVecCoors);

    #if __DEV
                                                                                                                            if(NULL_IN_SCRIPTING_LANGUAGE != iPedID)
		{
			const CPed *pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
			if(pPed)
				scriptAssertf(DistSquared(RCC_VEC3V(v), pPed->GetTransform().GetPosition()).Getf() < 100.0f, "TASK_PED_SLIDE_TO_COORD_HDG_RATE is moving a ped over 10m.");
		}
    #endif

    static dev_float fTargetRadius = 0.125f;  //CTaskMoveGoToPointAndStandStill::ms_fTargetRadius;

    CTaskSequenceList* pTaskList = rage_new
    CTaskSequenceList();
    CTaskMoveGoToPointAndStandStill* pGotoTask = rage_new
    CTaskMoveGoToPointAndStandStill(MOVEBLENDRATIO_WALK, v, fTargetRadius);
    pTaskList->AddTask(rage_new
    CTaskComplexControlMovement(pGotoTask));

    CTaskSlideToCoord* pTaskSlide = rage_new
    CTaskSlideToCoord(v, heading, speed);
    pTaskSlide->SetHeadingIncrement((DtoR * fHdgChangeRate));
    pTaskList->AddTask(pTaskSlide);
    CScriptPeds::GivePedScriptedTask(iPedID, pTaskList, SCRIPT_TASK_PED_SLIDE_TO_COORD, "TASK_PED_SLIDE_TO_COORD_HDG_RATE");
  }

  void CommandTaskPedSlideToCoord(int iPedID, const scrVector& scrVecCoors, float fHeadingDegs, float fSpeed) {
    CommandTaskPedSlideToCoordWithHeadingChangeRate(iPedID, scrVecCoors, fHeadingDegs, fSpeed, 0.0f);
  }

  //This command is not called by any include_command.sch but needs to be incorporated into CommandTaskPedSlideToCoordAndPlayAnim
  void CommandTaskPedSlideToCoordAndPlayAnimWithHeadingChangeRate(int iPedID,
                                                                  const scrVector& scrVecCoors,
                                                                  float fHeadingDegrees, float fSpeed,
                                                                  const char* AnimName, const char* AnimDictName,
                                                                  float fBlendDelta,
                                                                  bool bLooped, bool bExtractBackwardsVelocity, bool bExtractSidewaysVelocity, bool bHoldOnLastFrame,
                                                                  int iTime, float fHdgChangeRate) {
    while (fHeadingDegrees < 0.0f)
      fHeadingDegrees += 360.0f;
    while (fHeadingDegrees >= 360.0f)
      fHeadingDegrees -= 360.0f;
    const float heading = (DtoR * fHeadingDegrees);
    float speed = fSpeed;
    if (speed < 0) {
      speed = 0.1f;
    }
    s32 flags = 0;
    if (bLooped || (iTime > 0)) {
      flags |= APF_ISLOOPED;
    }
    if (bExtractBackwardsVelocity) {
      //flags |= APF_CANEXTRACTYVELOCITY; //These flags no longer exist: GS 09/12/2009
    }
    if (bHoldOnLastFrame == false) {
      flags |= APF_ISFINISHAUTOREMOVE;
    }
    if (bExtractSidewaysVelocity) {
      //scriptAssertf(bExtractBackwardsVelocity, "%s:TASK_PLAY_ANIM - If you want to extract sideways velocity you have to extract forwards velocity too", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      //flags |= APF_CANEXTRACTXVELOCITY; //These flags no longer exist: GS 09/12/2009
    }
    bool bRunInSequence = false;
    if (CTaskSequences::ms_iActiveSequence >= 0) {
      bRunInSequence = true;
    }
    Vector3 v(scrVecCoors);

    #if __DEV
                                                                                                                            const CPed *pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
		if(pPed)
			scriptAssertf(DistSquared(RCC_VEC3V(v), pPed->GetTransform().GetPosition()).Getf() < 100.0f, "TASK_PED_SLIDE_TO_COORD_HDG_RATE is moving a ped over 10m.");
    #endif

    static dev_float fTargetRadius = 0.125f;  //CTaskMoveGoToPointAndStandStill::ms_fTargetRadius;

    CTaskSequenceList* pTaskList = rage_new
    CTaskSequenceList();

    // JB: Slightly larger target radius, since peds were occasionally overshooting their target & then sliding back
    CTaskMoveGoToPointAndStandStill* pTaskGoto = rage_new
    CTaskMoveGoToPointAndStandStill(MOVEBLENDRATIO_WALK, v, fTargetRadius);
    pTaskList->AddTask(rage_new
    CTaskComplexControlMovement(pTaskGoto) );

    CTaskSlideToCoord* pTaskSlide;

    if (iTime > 0) {
      pTaskSlide = rage_new
      CTaskSlideToCoord(v, heading, speed, AnimName, AnimDictName, flags, fBlendDelta, bRunInSequence, iTime);
    } else {
      pTaskSlide = rage_new
      CTaskSlideToCoord(v, heading, speed, AnimName, AnimDictName, flags, fBlendDelta, bRunInSequence);
    }
    pTaskSlide->SetHeadingIncrement((DtoR * fHdgChangeRate));

    pTaskList->AddTask(pTaskSlide);

    CScriptPeds::GivePedScriptedTask(iPedID, pTaskList, SCRIPT_TASK_PED_SLIDE_TO_COORD_AND_PLAY_ANIM, "TASK_PED_SLIDE_TO_COORD_AND_PLAY_ANIM_HDG_RATE");
  }

  void CommandTaskPedSlideToCoordAndPlayAnim(
    int iPedID,
    const scrVector& scrVecCoors,
    float fHeadingDegrees, float fSpeed,
    const char* AnimName, const char* AnimDictName,
    float fBlendDelta,
    bool bLooped, bool bExtractBackwardsVelocity, bool bExtractSidewaysVelocity, bool bHoldOnLastFrame,
    int iTime) {
    CommandTaskPedSlideToCoordAndPlayAnimWithHeadingChangeRate(
      iPedID, scrVecCoors, fHeadingDegrees, fSpeed, AnimName, AnimDictName, fBlendDelta, bLooped,
      bExtractBackwardsVelocity, bExtractSidewaysVelocity, bHoldOnLastFrame, iTime, 0.0f);
  }

  void CommandTaskDrivePointRouteAdvanced(int iPedID, int iVehicleID, float fCruiseSpeed, int iMode, int VehicleModelHashKey, int iDrivingStyle) {
    CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);

    if (pVehicle) {
      fwModelId VehicleModelId;
      if (VehicleModelHashKey != DUMMY_ENTRY_IN_MODEL_ENUM_FOR_SCRIPTING_LANGUAGE) {
        CModelInfo::GetBaseModelInfoFromHashKey((u32) VehicleModelHashKey, &VehicleModelId);    //	ignores return value
        scriptAssertf(VehicleModelId.IsValid(), "%s:TASK_DRIVE_POINT_ROUTE_ADVANCED - this is not a valid model index", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      }
      CTask* pTask = rage_new
      CTaskDrivePointRoute(pVehicle, CTaskMoveFollowPointRoute::ms_pointRoute, fCruiseSpeed, iMode, VehicleModelId.GetModelIndex(), 7.0f, iDrivingStyle);  // 7.0 was -1.0 (finished radius)
      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_DRIVE_POINT_ROUTE_ADVANCED, "TASK_DRIVE_POINT_ROUTE_ADVANCED");
    }
  }

  void CommandTaskGuardAssignedDefensiveArea(int iPedIndex, const scrVector& vDefendPosition, float fHeading, float fMaxPatrolProximity, int nTimer) {
    if (SCRIPT_VERIFY((nTimer == -1) || (nTimer > 0), "TASK_GUARD_ASSIGNED_DEFENSIVE_AREA - Time must be -1 for infinite and > 0")) {
      Vector3 vDefendPosV3 = vDefendPosition;
      float fNewPedHeading = fHeading;
      float fTimer = ((float) nTimer) / 1000.0f;

      while (fNewPedHeading < 0.0f) {
        fNewPedHeading += 360.0f;
      }
      while (fNewPedHeading > 360.0f) {
        fNewPedHeading -= 360.0f;
      }

      CTask* pTask = rage_new
      CTaskStandGuard(vDefendPosV3, (DtoR * fNewPedHeading), fMaxPatrolProximity, GM_PatrolDefensiveArea, fTimer);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_GUARD_DEFENSIVE_AREA, "TASK_GUARD_ASSIGNED_DEFENSIVE_AREA");
    }
  }

  void CommandTaskGuardAngledDefensiveArea(int iPedIndex, const scrVector& vDefendPosition, float fHeading, float fMaxPatrolProximity, int iTime, const scrVector& sv1, const scrVector& sv2, float fDefensiveAreaWidth) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_GUARD_ANGLED_DEFENSIVE_AREA - Time must be -1 for infinite and > 0")) {
      scriptAssertf(fDefensiveAreaWidth >= CDefensiveArea::GetMinRadius(), "TASK_GUARD_ANGLED_DEFENSIVE_AREA - defensive area radius should be at least %.2f, but passed in - %.2f", CDefensiveArea::GetMinRadius(), fDefensiveAreaWidth);

      Vector3 vDefendPosV3 = vDefendPosition;
      float fNewPedHeading = fHeading;
      float fTimer = ((float) iTime) / 1000.0f;

      if (fNewPedHeading < 0.0f) {
        fNewPedHeading += 360.0f;
      }
      if (fNewPedHeading > 360.0f) {
        fNewPedHeading -= 360.0f;
      }

      Vector3 v1 = sv1;
      Vector3 v2 = sv2;

      CDefensiveArea area;
      area.Set(v1, v2, fDefensiveAreaWidth);

      CTask* pTask = rage_new
      CTaskSetAndGuardArea(vDefendPosV3, (DtoR * fNewPedHeading), fMaxPatrolProximity, GM_PatrolDefensiveArea, fTimer, area);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_GUARD_ANGLED_DEFENSIVE_AREA, "TASK_GUARD_ANGLED_DEFENSIVE_AREA");
    }
  }

  void CommandTaskGuardSphereDefensiveArea(int iPedIndex, const scrVector& vDefendPosition, float fHeading, float fMaxPatrolProximity, int iTime, const scrVector& sv1, float fDefensiveAreaRadius) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_GUARD_SPHERE_DEFENSIVE_AREA - Time must be -1 for infinite and > 0")) {
      Vector3 v1 = sv1;

      Vector3 vDefendPosV3 = vDefendPosition;
      float fNewPedHeading = fHeading;
      float fTimer = ((float) iTime) / 1000.0f;

      scriptAssertf(!vDefendPosV3.IsClose(VEC3_ZERO, SMALL_FLOAT), "TASK_GUARD_SPHERE_DEFENSIVE_AREA is tasking a ped to defend the origin.  This is is very likely an error.");

      scriptAssertf(fDefensiveAreaRadius >= CDefensiveArea::GetMinRadius(), "TASK_GUARD_SPHERE_DEFENSIVE_AREA - defensive area radius should be at least %.2f, but passed in - %.2f", CDefensiveArea::GetMinRadius(), fDefensiveAreaRadius);

      if (fNewPedHeading < 0.0f) {
        fNewPedHeading += 360.0f;
      }
      if (fNewPedHeading > 360.0f) {
        fNewPedHeading -= 360.0f;
      }

      CDefensiveArea area;
      area.SetAsSphere(v1, fDefensiveAreaRadius);

      CTask* pTask = rage_new
      CTaskSetAndGuardArea(vDefendPosV3, (DtoR * fNewPedHeading), fMaxPatrolProximity, GM_PatrolDefensiveArea, fTimer, area);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_GUARD_SPHERE_DEFENSIVE_AREA, "TASK_GUARD_SPHERE_DEFENSIVE_AREA");
    }
  }

  void CommandTaskGuardCurrentPosition(int iPedIndex, float fMaxPatrolProximity, float fDefensiveAreaRadius, bool bSetDefensiveArea) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);

    if (SCRIPT_VERIFY(pPed, "Invalid ped tasked with TASK_GUARD_CURRENT_POSITION")) {
      scriptAssertf(fDefensiveAreaRadius >= CDefensiveArea::GetMinRadius(), "TASK_GUARD_CURRENT_POSITION - defensive area radius should be at least %.2f, but passed in - %.2f", CDefensiveArea::GetMinRadius(), fDefensiveAreaRadius);

      Vector3 vDefendPosV3 = VEC3V_TO_VECTOR3(pPed->GetTransform().GetPosition());
      CDefensiveArea area;
      area.SetAsSphere(vDefendPosV3, fDefensiveAreaRadius);
      float fTimer = -1.0f;

      eGuardMode guardMode = bSetDefensiveArea ? GM_PatrolDefensiveArea : GM_PatrolProximity;

      CTask* pTask = rage_new
      CTaskSetAndGuardArea(vDefendPosV3, pPed->GetCurrentHeading(), fMaxPatrolProximity, guardMode, fTimer, area, true, bSetDefensiveArea);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_GUARD_CURRENT_POSITION, "TASK_GUARD_CURRENT_POSITION");
    }
  }

  // Stand guard at a position
  void CommandTaskStandGuard(int iPedIndex, const scrVector& vDefendPosition, float fHeading, const char* szContext) {
    Vector3 vDefendPosV3 = vDefendPosition;
    float fNewPedHeading = fHeading;

    fwAngle::LimitDegreeAngle(fNewPedHeading);

    if (strcmp(szContext, "Default") == 0) {
      CTask* pTask = rage_new
      CTaskStandGuardFSM(vDefendPosV3, (DtoR * fNewPedHeading));
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_STAND_GUARD, "TASK_STAND_GUARD");
    } else {

      s32 scenarioType = CScenarioManager::GetScenarioTypeFromHashKey(atStringHash(szContext));
      CTask* pTask = rage_new
      CTaskStandGuardFSM(vDefendPosV3, (DtoR * fNewPedHeading), scenarioType);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_STAND_GUARD, "TASK_STAND_GUARD");
    }
  }

  // Add a scripted cover point
  int CommandAddCoverPoint(const scrVector& scrVecCoors, float fDirection, s32 iUsage, s32 iHeight, s32 iArc, bool bIsPriority) {
    AI_LOG_WITH_ARGS("[Script] - Script %s is adding a coverpoint at position (%.2f, %.2f, %.2f)", CTheScripts::GetCurrentScriptName(), scrVecCoors.x, scrVecCoors.y, scrVecCoors.z);
    AI_LOG_STACK_TRACE(8);

    CScriptResource_Coverpoint coverPoint(Vector3(scrVecCoors), fDirection, iUsage, iHeight, iArc, bIsPriority);

    return CTheScripts::GetCurrentGtaScriptHandler()->RegisterScriptResourceAndGetRef(coverPoint);
  }

  // Remove a scripted cover point
  void CommandRemoveCoverPoint(int iIdentifier) {
    AI_LOG_WITH_ARGS("[Script] - Script %s is removing a cover point with ID %d", CTheScripts::GetCurrentScriptName(), iIdentifier);
    AI_LOG_STACK_TRACE(8);

    CTheScripts::GetCurrentGtaScriptHandler()->RemoveScriptResource(CGameScriptResource::SCRIPT_RESOURCE_COVERPOINT, iIdentifier);
  }

  bool CommandDoesScriptedCoverPointExistAtCoords(const scrVector& scrVecCoors) {
    CCoverPoint* pOverlappingCoverPoint = NULL;
    if (!CCoverPointsContainer::CheckIfNoOverlapAndGetGridCell(scrVecCoors, CCoverPoint::COVTYPE_SCRIPTED, NULL, pOverlappingCoverPoint, false)) {
      return true;
    } else {
      return false;
    }
  }

  scrVector CommandGetScriptedCoverPointCoords(int iIdentifier) {
    CScriptedCoverPoint* pScriptedCoverPoint = CTheScripts::GetScriptedCoverPointToModifyFromGUID(iIdentifier);
    if (SCRIPT_VERIFY(pScriptedCoverPoint, "GET_SCRIPTED_COVER_POINT_COORDS - Identifier is invalid")) {
      CCoverPoint* pCoverPoint = CCover::FindCoverPointWithIndex(pScriptedCoverPoint->GetScriptIndex());
      if (scriptVerifyf(pCoverPoint, "GET_SCRIPTED_COVER_POINT_COORDS - Cover Point %i Doesn't Exist", iIdentifier)) {
        Vector3 vCoverPos(Vector3::ZeroType);
        if (scriptVerifyf(pCoverPoint->GetCoverPointPosition(vCoverPos), "GET_SCRIPTED_COVER_POINT_COORDS - Couldn't get cover point coords for cover point with index %i", iIdentifier)) {
          return vCoverPos;
        }
      }
    }
    return VEC3_ZERO;
  }

  // Add a scripted cover area which will be appended to the streamed cover area array
  void CommandAddScriptedCoverArea(const scrVector& scrVecCoors, const float fRadius) {
    if (CPed * pPlayer = CGameWorld::FindLocalPlayer()) {
      Vector3 vFrom(scrVecCoors);

      Vector3 vDistance = VEC3V_TO_VECTOR3(pPlayer->GetTransformPtr()->GetPosition()) - vFrom;
      float fDistance2 = vDistance.Mag2();

      if (scriptVerifyf(fDistance2 < rage::square(200), "ADD_SCRIPTED_COVER_AREA - Scripted Area Cover distance from player is %fm, it must be within 200m to add.", vDistance.Mag())) {
        AI_LOG_WITH_ARGS("[Script] - Script %s is adding a scripted cover area at position (%.2f, %.2f, %.2f) with a radius of %.2f", CTheScripts::GetCurrentScriptName(), scrVecCoors.x, scrVecCoors.y, scrVecCoors.z, fRadius);

        CCover::AddNewScriptedCoverArea(scrVecCoors, fRadius);
      }
    }
  }

  /*int CommandGetScriptedCoverPointStatus( int iIdentifier )
	{
		CScriptedCoverPoint* pScriptedCoverPoint = CTheScripts::GetScriptedCoverPointToModifyFromGUID(iIdentifier);
		if (scriptVerifyf(pScriptedCoverPoint, "GET_SCRIPTED_COVER_POINT_STATUS - Cover Point %i Doesn't Exist", iIdentifier))
		{
			CCoverPoint* pCoverPoint = CCover::FindCoverPointWithIndex(pScriptedCoverPoint->GetScriptIndex());
			if (scriptVerifyf(pCoverPoint, "GET_SCRIPTED_COVER_POINT_STATUS - Cover Point %i Doesn't Exist", iIdentifier))
			{
				return pCoverPoint->GetStatus();
			}
		}
		return CCoverPoint::COVSTATUS_Invalid;
	}*/

  // Seek cover from the given position
  void CommandTaskSeekCoverFromPos(int PedIndex, const scrVector& scrVecCoors, int iTime, bool bAllowPeekingAndFiring) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SEEK_COVER_FROM_POS - Time must be -1 for infinite and > 0")) {
      float fTime = ((float) iTime) / 1000.0f;

      Vector3 vFrom(scrVecCoors);

      CTaskCover* pCoverTask = rage_new
      CTaskCover(CAITarget(vFrom), bAllowPeekingAndFiring ? 0 : CTaskCover::CF_DisableAimingAndPeeking);
      pCoverTask->SetSearchFlags(CTaskCover::CF_SeekCover | CTaskCover::CF_CoverSearchAny | CTaskCover::CF_ScriptedSeekCover);
      pCoverTask->SetTimeInCover(fTime);

      CScriptPeds::GivePedScriptedTask(PedIndex, pCoverTask, SCRIPT_TASK_SEEK_COVER_FROM_POS, "TASK_SEEK_COVER_FROM_POS");
    }
  }

  // Seek cover from the given ped
  void CommandTaskSeekCoverFromPed(int iPedIndex, int iOtherPedIndex, int iTime, bool bAllowPeekingAndFiring) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SEEK_COVER_FROM_PED - Time must be -1 for infinite and > 0")) {
      float fTime = ((float) iTime) / 1000.0f;

      const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedIndex);

      if (!pOtherPed)
        return;

      CTaskCover* pCoverTask = rage_new
      CTaskCover(CAITarget(pOtherPed), bAllowPeekingAndFiring ? 0 : CTaskCover::CF_DisableAimingAndPeeking);
      pCoverTask->SetSearchFlags(CTaskCover::CF_SeekCover | CTaskCover::CF_CoverSearchAny | CTaskCover::CF_ScriptedSeekCover);
      pCoverTask->SetTimeInCover(fTime);

      CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_SEEK_COVER_FROM_PED, "TASK_SEEK_COVER_FROM_PED");
    }
  }

  // Seek cover to the given cover point, away from the position specified
  void CommandTaskSeekCoverToCoverPoint(int iPedIndex, int iIdentifier, const scrVector& scrVecCoors, int iTime, bool bAllowPeekingAndFiring) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SEEK_COVER_TO_COVER_POINT - Time must be -1 for infinite and > 0")) {

      float fTime = ((float) iTime) / 1000.0f;

      CScriptedCoverPoint* pScriptedCoverPoint = CTheScripts::GetScriptedCoverPointToModifyFromGUID(iIdentifier, "TASK_SEEK_COVER_TO_COVER_POINT");

      if (scriptVerifyf(pScriptedCoverPoint, "Cover point with id %i didn't exist", iIdentifier)) {
        s32 iCoverIndex = pScriptedCoverPoint->GetScriptIndex();

        // Find the cover point specified by the identifier
        #if __ASSERT

                                                                                                                                CCoverPoint* pCoverPoint = CCover::FindCoverPointWithIndex( iCoverIndex );
				scriptAssertf(pCoverPoint, "%s:TASK_SEEK_COVER_TO_COVER_POINT - Invalid cover point specified", CTheScripts::GetCurrentScriptNameAndProgramCounter());
				Assert( pCoverPoint->GetType() == CCoverPoint::COVTYPE_SCRIPTED );
        #endif
        Vector3 vFrom(scrVecCoors);

        CTaskCover* pCoverTask = rage_new
        CTaskCover(CAITarget(vFrom), bAllowPeekingAndFiring ? 0 : CTaskCover::CF_DisableAimingAndPeeking);
        pCoverTask->SetSearchFlags(CTaskCover::CF_SeekCover | CTaskCover::CF_CoverSearchScripted | CTaskCover::CF_ScriptedSeekCover);
        pCoverTask->SetScriptedCoverIndex(iCoverIndex);
        pCoverTask->SetTimeInCover(fTime);

        CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_SEEK_COVER_TO_COVER_POINT, "TASK_SEEK_COVER_TO_COVER_POINT");
      }
    }
  }

  // Seek cover to the given coords, away from the position specified
  void CommandTaskSeekCoverToCoords(int iPedIndex, const scrVector& scrVecCoverCoors, const scrVector& scrVecFromCoors, int iTime, bool bAllowPeekingAndFiring) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_SEEK_COVER_TO_COVER_COORD - Time must be -1 for infinite and > 0")) {
      float fTime = ((float) iTime) / 1000.0f;

      Vector3 vCover(scrVecCoverCoors);
      Vector3 vFrom(scrVecFromCoors);

      CTaskCover* pCoverTask = rage_new
      CTaskCover(CAITarget(vFrom), bAllowPeekingAndFiring ? 0 : CTaskCover::CF_DisableAimingAndPeeking);
      pCoverTask->SetSearchFlags(CTaskCover::CF_SeekCover | CTaskCover::CF_CoverSearchByPos | CTaskCover::CF_ScriptedSeekCover);
      pCoverTask->SetSearchPosition(vCover);
      pCoverTask->SetTimeInCover(fTime);

      CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_SEEK_COVER_TO_COORDS, "TASK_SEEK_COVER_TO_COVER_COORD");
    }
  }

  CTaskCover* CreatePutPedDirectlyIntoCoverTask(const scrVector& vscStart, const CAITarget& aiTarget, int iTime, bool bAllowPeekingAndFiring, float fBlendInDuration, bool bForceInitialFacingDirection, bool bForceFaceLeft, int iIdentifier, bool bDoEntry) {
    scriptAssertf(fBlendInDuration < 10.0f, "Setting a long blend in duration (%.2f) are you sure you want the anims to blend in this slowly?", fBlendInDuration);
    float fTime = ((float) iTime) / 1000.0f;
    Vector3 vStart = vscStart;

    s32 iCoverFlags = bAllowPeekingAndFiring ? CTaskCover::CF_PutPedDirectlyIntoCover : CTaskCover::CF_PutPedDirectlyIntoCover | CTaskCover::CF_DisableAimingAndPeeking;

    if (bForceInitialFacingDirection) {
      aiDisplayf("TASK_PUT_PED_DIRECTLY_INTO_COVER called forcing ped to face %s at <<%.2f,%.2f,%.2f>>", bForceFaceLeft ? "Left" : "Right", vscStart.x, vscStart.y, vscStart.z);
      iCoverFlags |= CTaskCover::CF_SpecifyInitialHeading;
      if (bForceFaceLeft) {
        iCoverFlags |= CTaskCover::CF_FacingLeft;
      }
    }

    // Set whether we want to skip the cover entry anims
    if (!bDoEntry) {
      iCoverFlags |= CTaskCover::CF_SkipIdleCoverTransition;
    }

    CTaskCover* pCoverTask = rage_new
    CTaskCover(aiTarget, iCoverFlags);

    CScriptedCoverPoint* pScriptedCoverPoint = NULL;

    if (iIdentifier != NULL_IN_SCRIPTING_LANGUAGE) {
      pScriptedCoverPoint = CTheScripts::GetScriptedCoverPointToModifyFromGUID(iIdentifier, "TASK_PUT_PED_DIRECTLY_INTO_COVER");
    }

    if (pScriptedCoverPoint) {
      s32 iCoverIndex = pScriptedCoverPoint->GetScriptIndex();
      #if __ASSERT
                                                                                                                              // Find the cover point specified by the identifier
			CCoverPoint* pCoverPoint = CCover::FindCoverPointWithIndex( iCoverIndex );
			scriptAssertf(pCoverPoint, "%s:TASK_SEEK_COVER_TO_COVER_POINT - Invalid cover point specified", CTheScripts::GetCurrentScriptNameAndProgramCounter());
			Assert( pCoverPoint->GetType() == CCoverPoint::COVTYPE_SCRIPTED );
      #endif
      pCoverTask->SetSearchFlags(CTaskCover::CF_CoverSearchScripted | CTaskCover::CF_ScriptedSeekCover);
      pCoverTask->SetScriptedCoverIndex(iCoverIndex);
    } else {
      pCoverTask->SetSearchFlags(CTaskCover::CF_FindClosestPointAtStart | CTaskCover::CF_ScriptedSeekCover);
    }

    pCoverTask->SetBlendInDuration(fBlendInDuration);
    pCoverTask->SetSearchPosition(vStart);
    pCoverTask->SetTimeInCover(fTime);
    return pCoverTask;
  }

  // Seek cover from the given position
  void CommandTaskPutPedDirectlyIntoCover(int iPedIndex, const scrVector& vscStart, int iTime, bool bAllowPeekingAndFiring, float fBlendInDuration, bool bForceInitialFacingDirection, bool bForceFaceLeft, int iIdentifier, bool bDoEntry) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_PUT_PED_DIRECTLY_INTO_COVER - Time must be -1 for infinite and > 0")) {
      CTaskCover* pCoverTask = CreatePutPedDirectlyIntoCoverTask(vscStart, CAITarget(), iTime, bAllowPeekingAndFiring, fBlendInDuration, bForceInitialFacingDirection, bForceFaceLeft, iIdentifier, bDoEntry);
      CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_COVER, "TASK_PUT_PED_DIRECTLY_INTO_COVER");
    }
  }

  bool GetAiTarget(CAITarget& target, int iOtherPedIndex, int iTargetVehicleIndex, const scrVector& vscTargetCoords) {
    Vector3 TempVec, CentreVec, VecDiff;
    const CPed* pSecondPed;
    const CVehicle* pSecondVehicle;

    TempVec = Vector3(vscTargetCoords);

    const CEntity* pTargetEntity = NULL;
    if (iOtherPedIndex != NULL_IN_SCRIPTING_LANGUAGE)  //	>= 0)
    {
      if (SCRIPT_VERIFY(iTargetVehicleIndex == NULL_IN_SCRIPTING_LANGUAGE, "TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET - Can't have a target ped and a target car")) {
        pSecondPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iOtherPedIndex);
        if (!pSecondPed)
          return false;

        pTargetEntity = pSecondPed;
      } else {
        return false;
      }
    } else {
      pSecondPed = NULL;
    }
    if (iTargetVehicleIndex != NULL_IN_SCRIPTING_LANGUAGE)  //	>= 0)
    {
      if (SCRIPT_VERIFY(iOtherPedIndex == NULL_IN_SCRIPTING_LANGUAGE, "TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET - Can't have a target ped and a target car")) {
        pSecondVehicle = CTheScripts::GetEntityToQueryFromGUID<CVehicle>(iTargetVehicleIndex);
        if (!SCRIPT_VERIFY(pSecondVehicle, "TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET - Target vehicle doesn't exist"))
          return false;

        pTargetEntity = pSecondVehicle;
      } else {
        return false;
      }
    } else {
      pSecondVehicle = NULL;
    }

    // Set up the arguments
    if (pTargetEntity) {
      target.SetEntityAndOffset(pTargetEntity, TempVec);
    } else {
      target.SetPosition(TempVec);
    }
    return true;
  }

  // Seek cover from the given position
  void CommandTaskPutPedDirectlyIntoCoverFromTarget(int iPedIndex, int iOtherPedIndex, int iTargetVehicleIndex, const scrVector& vscTargetCoords, const scrVector& vscStart, int iTime, bool bAllowPeekingAndFiring, float fBlendInDuration, bool bForceInitialFacingDirection, bool bForceFaceLeft, int iIdentifier, bool bDoEntry) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET - Time must be -1 for infinite and > 0")) {
      CAITarget target;
      if (GetAiTarget(target, iOtherPedIndex, iTargetVehicleIndex, vscTargetCoords)) {
        CTaskCover* pCoverTask = CreatePutPedDirectlyIntoCoverTask(vscStart, target, iTime, bAllowPeekingAndFiring, fBlendInDuration, bForceInitialFacingDirection, bForceFaceLeft, iIdentifier, bDoEntry);
        CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET, "TASK_PUT_PED_DIRECTLY_INTO_COVER");
      }
    }
  }

  void CommandTaskWarpPedDirectlyIntoCover(const int iPedIndex, const int iTime, const bool bAllowPeekingAndFiring, const bool bForceInitialFacingDirection, const bool bForceFaceLeft, const int iIdentifier) {
    if (SCRIPT_VERIFY((iTime == -1) || (iTime > 0), "TASK_WARP_PED_DIRECTLY_INTO_COVER - Time must be -1 for infinite and > 0") &&
        SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedIndex, "TASK_WARP_PED_DIRECTLY_INTO_COVER - Can't warp a NULL ped")) {
      if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES)) {
        Vector3 vPedPosition = VEC3V_TO_VECTOR3(pPed->GetTransform().GetPosition());

        CTaskCover* pCoverTask = CreatePutPedDirectlyIntoCoverTask(vPedPosition, CAITarget(), iTime, bAllowPeekingAndFiring, 0.0f, bForceInitialFacingDirection, bForceFaceLeft, iIdentifier, false);
        pCoverTask->SetCoverFlag(CTaskCover::CF_WarpPedToCoverPosition);
        CScriptPeds::GivePedScriptedTask(iPedIndex, pCoverTask, SCRIPT_TASK_WARP_PED_DIRECTLY_INTO_COVER, "TASK_WARP_PED_DIRECTLY_INTO_COVER");
      }
    }
  }

  void CommandSetCoverTarget(int iPedIndex, int iOtherPedIndex, int iTargetVehicleIndex, const scrVector& vscTargetCoords) {
    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iPedIndex, "SET_COVER_TARGET - Can't have a NULL ped")) {
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex);
      if (pPed && SCRIPT_VERIFY(!pPed->IsNetworkClone(), "SET_COVER_TARGET - Can't call this command on a network clone!")) {
        CAITarget aiTarget;
        if (GetAiTarget(aiTarget, iOtherPedIndex, iTargetVehicleIndex, vscTargetCoords)) {
          CTaskCover* pCoverTask = static_cast<CTaskCover*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_COVER));
          if (pCoverTask) {
            CWeaponTarget& weaponTarget = pCoverTask->GetWeaponTarget();
            const CEntity* pEntity = aiTarget.GetEntity();
            if (pEntity) {
              Vector3 vOffset(Vector3::ZeroType);
              if (aiTarget.GetPositionOffset(vOffset)) {
                weaponTarget.SetEntityAndOffset(pEntity, vOffset);
              } else {
                weaponTarget.SetEntity(pEntity);
              }
            } else {
              Vector3 vPosition(Vector3::ZeroType);
              if (aiTarget.GetPosition(vPosition)) {
                weaponTarget.SetPosition(vPosition);
              }
            }
          }
        }
      }
    }
  }

  // Exit cover
  void CommandTaskExitCover(int iPedIndex, int iExitType, const scrVector& vscTargetCoords) {
    s32 iFlags = 0;
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, 0);
    if (pPed) {
      // Early out if called on network clone or ped doesn't have a cover point
      if (!SCRIPT_VERIFY(!pPed->IsNetworkClone(), "SCRIPT_TASK_EXIT_COVER - Cannot call on network clone")) {
        return;
      }

      if (!SCRIPT_VERIFY(pPed->GetCoverPoint(), "SCRIPT_TASK_EXIT_COVER - Called on a ped without a cover point")) {
        return;
      }

      if (pPed->GetPedResetFlag(CPED_RESET_FLAG_InCoverFacingLeft)) {
        iFlags |= CTaskCover::CF_FacingLeft;
      }
    }

    CAITarget target;

    if (iExitType == CTaskCover::FE_Aim) {
      target.SetPosition(vscTargetCoords);
    }

    // Maybe given as part of a sequence, can't detect if ped will be in cover now if this is the case
    CScriptPeds::GivePedScriptedTask(iPedIndex, rage_new
    CTaskCover(target, iFlags, (CTaskCover::eForcedExit) iExitType), SCRIPT_TASK_EXIT_COVER, "SCRIPT_TASK_EXIT_COVER");
  }

  // Seek cover from the given position
  void CommandTaskPutPedDirectlyIntoMelee(int iPedIndex, int iTargetIndex, float fBlendInDuration, float fTimeInMelee, float fStrafePhaseSync, const int iAiCombatFlags) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex);
    if (pPed) {
      // Force the default unarmed weapon
      CPedWeaponManager* pWeaponManager = pPed->GetWeaponManager();
      if (pWeaponManager) {
        const CWeaponInfo* pWeaponInfo = pWeaponManager->GetEquippedWeaponInfo();
        if (pWeaponInfo && !pWeaponInfo->GetIsMelee())
          pWeaponManager->EquipWeapon(pPed->GetDefaultUnarmedWeaponHash(), -1, true);
      }

      CTaskMotionPed* pCurrentMotionTask = static_cast<CTaskMotionPed*>(pPed->GetPedIntelligence()->GetTaskManager()->FindTaskByTypeActive(PED_TASK_TREE_MOTION, CTaskTypes::TASK_MOTION_PED));
      if (pCurrentMotionTask) {
        pCurrentMotionTask->SetTaskFlag(CTaskMotionPed::PMF_SkipStrafeIntroAnim);
        pCurrentMotionTask->SetInstantStrafePhaseSync(fStrafePhaseSync);
        pCurrentMotionTask->SetStrafeDurationOverride(fBlendInDuration);
      }

      CPed* pTargetPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iTargetIndex);

      aiTask* pCombatTask = NULL;
      if (pPed->IsAPlayerPed()) {
        u32 uMeleeFlags = CTaskMelee::MF_ShouldBeInMeleeMode | CTaskMelee::MF_AllowStrafeMode;

        // Would the invoker like to keep the player in strafe?
        // NOTE: they also need to set CPED_RESET_FLAG_ForcePedToStrafe
        if (fTimeInMelee < 0.0f)
          uMeleeFlags |= CTaskMelee::MF_ForceStrafe;

        pCombatTask = rage_new
        CTaskMelee(NULL, pTargetPed, uMeleeFlags, CSimpleImpulseTest::ImpulseNone, (s32)(fTimeInMelee * 1000.0f));
      } else {
        CTaskThreatResponse* pTask = rage_new
        CTaskThreatResponse(pTargetPed);
        pTask->SetThreatResponseOverride(CTaskThreatResponse::TR_Fight);
        pTask->SetConfigFlagsForCombat(CTaskCombat::ComF_DisableAimIntro | CTaskCombat::ComF_MeleeAnimPhaseSync | iAiCombatFlags);
        pTask->SetConfigFlags(CTaskThreatResponse::CF_CanFightArmedPedsWhenNotArmed);
        pCombatTask = pTask;
      }

      CScriptPeds::GivePedScriptedTask(iPedIndex, pCombatTask, SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_MELEE, "SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_MELEE");
    }
  }

  void CommandSetDriveTaskCruiseSpeed(int iPedIndex, float CruiseSpeed) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK & CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);

    if (pPed) {
      if (SCRIPT_VERIFY(pPed->GetMyVehicle() && pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle), "SET_DRIVE_TASK_CRUISE_SPEED - Ped is not in car")) {
        if (SCRIPT_VERIFY(pPed->GetMyVehicle()->GetDriver() == pPed, "SET_DRIVE_TASK_CRUISE_SPEED - Ped is not driver of car")) {
          if (NetworkUtils::IsNetworkCloneOrMigrating(pPed)) {
            CScriptEntityStateChangeEvent::CSettingOfDriveTaskCruiseSpeed parameters(CruiseSpeed);
            CScriptEntityStateChangeEvent::Trigger(pPed->GetNetworkObject(), parameters);
          } else {
            CTaskVehicleMissionBase* pCarTask = pPed->GetMyVehicle()->GetIntelligence()->GetActiveTask();
            if (pCarTask) {
              pCarTask->SetCruiseSpeed(CruiseSpeed);
            }
          }
        }
      }
    }
  }

  void CommandSetDriveTaskMaxCruiseSpeed(int iPedIndex, float MaxCruiseSpeed) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex);

    if (pPed) {
      if (SCRIPT_VERIFY(pPed->GetMyVehicle() && pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle), "SET_DRIVE_TASK_MAX_CRUISE_SPEED - Ped is not in car")) {
        if (SCRIPT_VERIFY(pPed->GetMyVehicle()->GetDriver() == pPed, "SET_DRIVE_TASK_MAX_CRUISE_SPEED - Ped is not driver of car")) {
          CTaskVehicleMissionBase* pCarTask = pPed->GetMyVehicle()->GetIntelligence()->GetActiveTask();
          if (pCarTask)
            pCarTask->SetMaxCruiseSpeed((u8) MaxCruiseSpeed);
        }
      }
    }
  }

  void CommandSetDriveTaskDrivingStyle(int iPedIndex, int iDrivingFlags) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex);

    if (pPed) {
      if (SCRIPT_VERIFY(pPed->GetMyVehicle() && pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle), "SET_DRIVE_TASK_DRIVING_STYLE - Ped is not in car")) {
        if (SCRIPT_VERIFY(pPed->GetMyVehicle()->GetDriver() == pPed, "SET_DRIVE_TASK_DRIVING_STYLE - Ped is not driver of car")) {
          CTaskVehicleMissionBase* pCarTask = pPed->GetMyVehicle()->GetIntelligence()->GetActiveTask();
          if (pCarTask)
            pCarTask->SetDrivingFlags(iDrivingFlags);
        }
      }
    }
  }

  void CommandSetPursueTaskIdealDistance(int iPedIndex, float fIdealDistance) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex);

    if (pPed) {
      if (SCRIPT_VERIFY(pPed->GetMyVehicle() && pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_InVehicle), "SET_PURSUE_TASK_IDEAL_DISTANCE - Ped is not in car")) {
        if (SCRIPT_VERIFY(pPed->GetMyVehicle()->GetDriver() == pPed, "SET_PURSUE_TASK_IDEAL_DISTANCE - Ped is not driver of car")) {
          CTaskVehiclePursue* pPursueTask = static_cast<CTaskVehiclePursue*>(pPed->GetMyVehicle()->GetIntelligence()->GetTaskManager()->FindTaskByTypeActive(VEHICLE_TASK_TREE_PRIMARY, CTaskTypes::TASK_VEHICLE_PURSUE));
          if (SCRIPT_VERIFY(pPursueTask, "SET_PURSUE_TASK_IDEAL_DISTANCE - Vehicle is not running TASK_VEHICLE_PURSUE")) {
            pPursueTask->SetIdealDistance(fIdealDistance);
          }
        }
      }
    }
  }

  // Add a cover blocking area
  void CommandAddCoverBlockingArea(const scrVector& vStart, const scrVector& vEnd, bool bBlockObjects, bool bBlockVehicles, bool bBlockMap, bool bBlockPlayer) {
    Vector3 vStartV3 = vStart;
    Vector3 vEndV3 = vEnd;
    CCover::AddCoverBlockingArea(vStartV3, vEndV3, bBlockObjects, bBlockVehicles, bBlockMap, bBlockPlayer);
  }

  // Add a scripted cover point
  void CommandFlushCoverBlockingAreas() {
    CCover::FlushCoverBlockingAreas();
  }

  // Remove any scripted cover points that contain the specified position.
  void CommandRemoveCoverBlockingAreasAtPosition(const scrVector& vPosition) {
    Vector3 vPositionV3 = vPosition;
    CCover::RemoveCoverBlockingAreasAtPosition(vPositionV3);
  }

  // Remove any scripter cover points that have the specified params.
  void CommandRemoveSpecificCoverBlockingAreas(const scrVector& vStart, const scrVector& vEnd, bool bBlockObjects, bool bBlockVehicles, bool bBlockMap, bool bBlockPlayer) {
    Vector3 vStartV3 = vStart;
    Vector3 vEndV3 = vEnd;
    CCover::RemoveSpecificCoverBlockingArea(vStartV3, vEndV3, bBlockObjects, bBlockVehicles, bBlockMap, bBlockPlayer);
  }

  void CommandTaskStartScenarioInPlace(int PedIndex, const char* szScenario, int iTimeToLeave, bool playIntro) {
    s32 type = CScenarioManager::GetScenarioTypeFromName(szScenario);

    if (scriptVerifyf(type != Scenario_Invalid, "TASK_START_SCENARIO_IN_PLACE: Unknown scenario type (%s)", szScenario)) {
      int flags = CTaskUseScenario::SF_StartInCurrentPosition;
      if (!playIntro) {
        flags |= CTaskUseScenario::SF_SkipEnterClip;
      }
      if (iTimeToLeave < 0) {
        flags |= CTaskUseScenario::SF_IdleForever;
      }
      CTaskUseScenario* pTask = rage_new
      CTaskUseScenario(type, flags);

      #if __ASSERT
                                                                                                                              //If assigning a task with a prop, make sure the ped has an inventory
			if (PedIndex != NULL_IN_SCRIPTING_LANGUAGE)
			{
				CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
				scriptAssertf(!pTask->GetScenarioInfo().HasProp() || pPed->GetInventory(), "%s: Attempting to assign a ped a scenario (%s) that has a prop, but the ped(%s) has no inventory", CTheScripts::GetCurrentScriptNameAndProgramCounter(), pTask->GetScenarioInfo().GetName(), pPed->GetModelName());
			}
      #endif

      if (iTimeToLeave > 0) {
        float fTimeToLeave = (float) iTimeToLeave;
        pTask->SetTimeToLeave(fTimeToLeave / 1000.0f);
      }
      pTask->CreateProp();
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_START_SCENARIO_IN_PLACE, "TASK_START_SCENARIO_IN_PLACE");
    }
  }

  void CommandTaskStartScenarioAtPosition(int iPedIndex, const char* szScenario, const scrVector& svScenarioPlace, float fHeading, int iTimeToLeave, bool playIntro, bool bWarp) {
    Vector3 vScenarioPoint = svScenarioPlace;
    s32 type = CScenarioManager::GetScenarioTypeFromName(szScenario);
    if (scriptVerifyf(type != Scenario_Invalid, "TASK_START_SCENARIO_AT_POSITION: Unknown scenario type (%s)", szScenario)) {
      int flags = 0;
      if (bWarp) {
        flags |= CTaskUseScenario::SF_Warp;
      }
      if (!playIntro) {
        flags |= CTaskUseScenario::SF_SkipEnterClip;
      }
      if (iTimeToLeave < 0) {
        flags |= CTaskUseScenario::SF_IdleForever;
      }
      CTaskUseScenario* pTask = rage_new
      CTaskUseScenario(type, vScenarioPoint, fwAngle::LimitRadianAngle((DtoR * fHeading)), flags);
      if (iTimeToLeave > 0) {
        float fTimeToLeave = (float) iTimeToLeave;
        pTask->SetTimeToLeave(fTimeToLeave / 1000.0f);
      }
      pTask->CreateProp();
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_START_SCENARIO_AT_POSITION, "TASK_START_SCENARIO_AT_POSITION");
    }
  }

  void CommandTaskStartVehicleScenario(int iPedIndex, int iVehicleIndex, const char* szScenario, bool bWarp) {
    if (iPedIndex != NULL_IN_SCRIPTING_LANGUAGE && iVehicleIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      CVehicle* pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleIndex);
      if (!pVehicle)
        return;

      s32 type = CScenarioManager::GetScenarioTypeFromName(szScenario);
      if (scriptVerifyf(type != Scenario_Invalid, "TASK_START_VEHICLE_SCENARIO: Unknown scenario type (%s)", szScenario)) {
        scriptAssertf(SCENARIOINFOMGR.GetScenarioInfoByIndex(type) && SCENARIOINFOMGR.GetScenarioInfoByIndex(type)->GetIsClass<CScenarioParkedVehicleInfo>(), "Invalid Vehicle Scenario");
        CTaskParkedVehicleScenario* pTask = rage_new
        CTaskParkedVehicleScenario(type, pVehicle, bWarp);
        CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_START_VEHICLE_SCENARIO, "TASK_START_VEHICLE_SCENARIO");
      }
    }
  }

  bool CommandDoesScenarioExistInArea(const scrVector& svScenarioPlace, float fMaxRange, bool bMustBeFree) {
    Vector3 vScenarioPoint = svScenarioPlace;
    CPedPopulation::GetScenarioPointInAreaArgs args(RCC_VEC3V(vScenarioPoint), fMaxRange);
    args.m_MustBeFree = bMustBeFree;
    args.m_CheckPopulation = bMustBeFree;
    CScenarioPoint* pScenarioPoint = NULL;

    if (!CPedPopulation::GetScenarioPointInArea(args, &pScenarioPoint)) {
      return false;
    }
    return true;
  }

  bool CommandDoesScenarioOfTypeExistInArea(const scrVector& svScenarioPlace, const char* szScenario, float fMaxRange, bool bMustBeFree) {
    Vector3 vScenarioPoint = svScenarioPlace;
    CPedPopulation::GetScenarioPointInAreaArgs args(RCC_VEC3V(vScenarioPoint), fMaxRange);
    args.m_MustBeFree = bMustBeFree;
    args.m_CheckPopulation = bMustBeFree;
    args.m_NumTypes = 1;
    s32 type = CScenarioManager::GetScenarioTypeFromName(szScenario);
    args.m_Types = &type;

    scriptAssertf(type != Scenario_Invalid, "DOES_SCENARIO_OF_TYPE_EXIST_IN_AREA: Unknown scenario type (%s)", szScenario);

    CScenarioPoint* pScenarioPoint = NULL;
    if (!CPedPopulation::GetScenarioPointInArea(args, &pScenarioPoint)) {
      return false;
    }
    return true;
  }

  bool CommandIsScenarioOccupied(const scrVector& svScenarioPlace, float fMaxRange, bool onlyUsersActuallyAtScenario) {
    Vector3 vScenarioPoint = svScenarioPlace;
    CScenarioPoint* pScenarioPoint = NULL;

    CPedPopulation::GetScenarioPointInAreaArgs args(RCC_VEC3V(vScenarioPoint), fMaxRange);
    args.m_MustBeFree = false;
    args.m_CheckPopulation = false;

    if (!CPedPopulation::GetScenarioPointInArea(args, &pScenarioPoint)) {
      return false;
    }

    // If onlyUsersActuallyAtScenario is set, we need to loop over the peds later.
    bool needToCheckPeds = onlyUsersActuallyAtScenario;

    // Note: CPedPopulation::IsEffectInUse() is a bit unfortunate in how it always
    // returns true if the scenario is attached to anything but a CObject. This is
    // different than how the check is done in GetScenarioPointInArea()
    // and GeneratePedsFromScenarioPointList(), where a scenario attached to something
    // else is considered not in use. Thus, we check the type here.
    if (!pScenarioPoint->GetEntity() || pScenarioPoint->GetEntity()->GetIsTypeObject()) {
      if (!CPedPopulation::IsEffectInUse(*pScenarioPoint)) {
        return false;
      }
    } else if (pScenarioPoint->GetEntity() && !pScenarioPoint->GetEntity()->GetIsTypeObject()) {
      // Since we don't keep track of use for non-CObject entities,
      // we need to check the ped tasks.
      needToCheckPeds = true;
    }

    if (needToCheckPeds) {
      int scenarioTypeFoundInLocation = pScenarioPoint->GetScenarioTypeVirtualOrReal();

      bool isVirtual = SCENARIOINFOMGR.IsVirtualIndex(scenarioTypeFoundInLocation);

      bool foundNearby = false;

      CPed::Pool* pool = CPed::GetPool();
      const int maxPeds = pool->GetSize();
      for (int i = 0; i < maxPeds; i++) {
        CPed* pPed = pool->GetSlot(i);
        if (!pPed) {
          continue;
        }

        // Note: for virtual scenario types, we won't ever match GetScenarioType(). We could loop over
        // all the real types, but the code below looks like it may work well enough without that.
        if (!isVirtual && scenarioTypeFoundInLocation != CPed::GetScenarioType(*pPed)) {
          continue;
        }

        const CTaskUseScenario* pTaskUseScenario = static_cast<const CTaskUseScenario*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
        if (pTaskUseScenario) {
          if (pTaskUseScenario->GetScenarioPoint() != pScenarioPoint) {
            continue;
          }

          const int taskState = pTaskUseScenario->GetState();
          if (onlyUsersActuallyAtScenario && (taskState == CTaskUseScenario::State_Start || taskState == CTaskUseScenario::State_GoToStartPosition)) {
            // In this case, we certainly appear to have found the ped that's considered the user
            // of the scenario point, but it hasn't actually arrived yet. In that case, we don't consider
            // the scenario occupied, so we return false.
            return false;
          } else {
            // Found the true user of this scenario.
            return true;
          }
        } else if (!foundNearby && !isVirtual) {
          // Somehow, according to CPed::GetScenarioType(), we are using a scenario of the type we are looking for,
          // but we can't find the CTaskUseScenario. It might be possible for this to happen for some scenario that's
          // using a different task, or perhaps in some networking situation, so we try to fall back on a distance
          // check.

          static float s_DistThresholdSq = square(1.0f);  // MAGIC!

          const Vec3V scenarioWorldPosV = pScenarioPoint->GetPosition();
          const Vec3V pedPosV = pPed->GetTransform().GetPosition();
          const ScalarV distThresholdSqV = LoadScalar32IntoScalarV(s_DistThresholdSq);
          const ScalarV distSqV = DistSquared(scenarioWorldPosV, pedPosV);
          if (IsLessThanAll(distSqV, distThresholdSqV)) {
            // This ped is close, set this variable. Note that we don't return true right away here,
            // because we might still find a ped that's actually using this scenario through a CTaskUseScenario,
            // in which case returning false is still an option.
            foundNearby = true;
          }
        }
      }

      if (foundNearby) {
        // We didn't find anybody running a CTaskUseScenario on this scenario, but we found
        // somebody using a scenario of the right type, close enough to the point.
        return true;
      } else {
        // Nobody nearby is using a scenario of this type.
        return false;
      }
    } else {
      return true;
    }
  }

  bool CommandPedHasUseScenarioTask(int iPedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);
    if (SCRIPT_VERIFY(pPed, "PedIndex was invalid for PED_HAS_USE_SCENARIO_TASK!")) {
      CTaskUseScenario* pTaskScenario = static_cast<CTaskUseScenario*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
      if (pTaskScenario) {
        return true;
      }
    }

    return false;
  }

  void CommandOverrideTaskedScenarioBaseAnim(int iPedIndex, const char* clipSet, const char* clip) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);
    if (SCRIPT_VERIFY(pPed, "PedIndex was invalid for OVERRIDE_TASKED_SCENARIO_BASE_ANIM!")) {
      CTaskUseScenario* pTaskScenario = static_cast<CTaskUseScenario*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
      if (pTaskScenario) {
        fwMvClipSetId setId(clipSet);
        fwMvClipId clipId(clip);
        pTaskScenario->OverrideBaseClip(setId, clipId);
      }

      // if we are already running the ambient clips task then we should set the override on there as well.
      CTaskAmbientClips* pTaskAmbientClips = static_cast<CTaskAmbientClips*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AMBIENT_CLIPS));
      if (pTaskAmbientClips) {
        fwMvClipSetId setId(clipSet);
        fwMvClipId clipId(clip);
        pTaskAmbientClips->OverrideBaseClip(setId, clipId);
      }
    }
  }

  void CommandPlayAnimOnRunnningScenario(int iPedIndex, const char* clipSet, const char* clip) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);
    if (SCRIPT_VERIFY(pPed, "PedIndex was invalid for TASK_SCENARIO_PLAY_AS_IDLE!")) {
      CTaskAmbientClips* pTaskAmbientClips = static_cast<CTaskAmbientClips*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AMBIENT_CLIPS));
      if (SCRIPT_VERIFY(pTaskAmbientClips, "Ped is currently not running the right task to run TASK_SCENARIO_PLAY_AS_IDLE")) {
        fwMvClipSetId setId(clipSet);
        fwMvClipId clipId(clip);
        if (SCRIPT_VERIFYF(fwClipSetManager::GetClipSet(setId) != NULL, "ClipSet %s did not exist.  Are you sure it is in clip_sets.xml?", clipSet)) {
          pTaskAmbientClips->PlayAnimAsIdle(setId, clipId);
        }
      }
    }
  }

  int GetScenarioGroupHelper(const char* groupName, const char* ASSERT_ONLY(cmdName), bool ASSERT_ONLY(mayNotExist)) {
    int groupId = CScenarioPointManager::kNoGroup;
    if (groupName && groupName[0]) {
      groupId = SCENARIOPOINTMGR.FindGroupByNameHash(atHashString(groupName), true);
    }
    scriptAssertf(groupId != CScenarioPointManager::kNoGroup || mayNotExist, "%s used with group name %s, which doesn't exist.", cmdName, groupName ? groupName : "<NULL>");
    return groupId;
  }

  bool CommandDoesScenarioGroupExist(const char* groupName) {
    return GetScenarioGroupHelper(groupName, "DOES_SCENARIO_GROUP_EXIST", true) != CScenarioPointManager::kNoGroup;
  }

  bool CommandIsScenarioGroupEnabled(const char* groupName) {
    int groupId = GetScenarioGroupHelper(groupName, "IS_SCENARIO_GROUP_ENABLED", false);
    if (groupId != CScenarioPointManager::kNoGroup) {
      const int groupIndex = groupId - 1;
      return SCENARIOPOINTMGR.IsGroupEnabled(groupIndex);
    }
    return false;
  }

  void CommandSetScenarioGroupEnabled(const char* groupName, bool enabled) {
    int groupId = GetScenarioGroupHelper(groupName, "IS_SCENARIO_GROUP_ENABLED", false);
    if (groupId != CScenarioPointManager::kNoGroup) {
      const int groupIndex = groupId - 1;
      SCENARIOPOINTMGR.SetGroupEnabled(groupIndex, enabled);
    }
  }

  void CommandResetScenarioGroupsEnabled() {
    SCENARIOPOINTMGR.ResetGroupsEnabledToDefaults();
  }

  void CommandSetExclusiveScenarioGroup(const char* groupName) {
    int groupId = GetScenarioGroupHelper(groupName, "SET_EXCLUSIVE_SCENARIO_GROUP", false);
    if (groupId != CScenarioPointManager::kNoGroup) {
      const int groupIndex = groupId - 1;
      SCENARIOPOINTMGR.SetExclusivelyEnabledScenarioGroupIndex(groupIndex);
    }
  }

  void CommandResetExclusiveScenarioGroup() {
    SCENARIOPOINTMGR.SetExclusivelyEnabledScenarioGroupIndex(-1);
  }

  int GetScenarioTypeHelper(const char* typeName, const char* ASSERT_ONLY(cmdName)) {
    int ret = -1;
    if (typeName && typeName[0]) {
      const u32 hash = atStringHash(typeName);

      // Note: this is currently an O(n) operation.
      ret = SCENARIOINFOMGR.GetScenarioIndex(hash, false);
    }

    scriptAssertf(ret >= 0, "%s used with type name %s, which doesn't exist.", cmdName, typeName ? typeName : "<NULL>");

    return ret;
  }

  void CommandForceScenarioGroupPriority(const char* groupName, bool bIsHighPriority) {
    int groupId = GetScenarioGroupHelper(groupName, "FORCE_SCENARIO_GROUP_PRIORITY", true);
    if (groupId != CScenarioPointManager::kNoGroup) {
      CScenarioPointPriorityManager::GetInstance().ForceScenarioPointGroupPriority((u8) groupId, bIsHighPriority);
    }
  }

  void CommandResetScenarioGroupsPriority() {
    CScenarioPointPriorityManager::GetInstance().RestoreGroupsToOriginalPriorities();
  }

  bool CommandIsScenarioTypeEnabled(const char* typeName) {
    int typeIndex = GetScenarioTypeHelper(typeName, "IS_SCENARIO_TYPE_ENABLED");
    if (typeIndex >= 0) {
      return SCENARIOINFOMGR.IsScenarioTypeEnabled(typeIndex);
    }
    return true;
  }

  void CommandSetScenarioTypeEnabled(const char* typeName, bool enabled) {
    int typeIndex = GetScenarioTypeHelper(typeName, "SET_SCENARIO_TYPE_ENABLED");
    if (typeIndex >= 0) {
      SCENARIOINFOMGR.SetScenarioTypeEnabled(typeIndex, enabled);
    }
  }

  void CommandResetScenarioTypesEnabled() {
    SCENARIOINFOMGR.ResetScenarioTypesEnabledToDefaults();
  }

  void CommandSuppressNormalScenarioExitsNextFrame() {
    CScenarioManager::SetScenarioExitsSuppressed(true);
  }

  void CommandSuppressScenarioAttractionNextFrame() {
    CScenarioManager::SetScenarioAttractionSuppressed(true);
  }

  void CommandSuppressBreakoutScenarioExitsNextFrame() {
    CScenarioManager::SetScenarioBreakoutExitsSuppressed(true);
  }

  CTask* CommonUseNearestScenarioToPos(CPed* pPed, const scrVector& svScenarioPlace, float fMaxRange, bool bWarp, bool ASSERT_ONLY(bCalledFromTask), int iTimeToLeave, bool bMustBeTrainScenario) {
    Vector3 vScenarioPoint = svScenarioPlace;
    CScenarioPoint* pScenarioPoint = NULL;
    CPedPopulation::GetScenarioPointInAreaArgs args(RCC_VEC3V(vScenarioPoint), fMaxRange);
    args.m_MustBeFree = true;
    args.m_PedOrDummyToUsePoint = pPed;
    args.m_Random = false;
    args.m_MustBeAttachedToTrain = bMustBeTrainScenario;
    bool bCheckPeds = false;
    if (!CPedPopulation::GetScenarioPointInArea(args, &pScenarioPoint)) {
      // do it again and process it further...the actual use count on the scenario point isn't
      // always what we want if they are going to the point on a long chain and the player is much closer
      args.m_MustBeFree = false;
      args.m_CheckPopulation = false;

      if (!CPedPopulation::GetScenarioPointInArea(args, &pScenarioPoint)) {
        #if __ASSERT
                                                                                                                                if( !bCalledFromTask )
					scriptAssertf(0, "%s:TASK_USE_NEAREST_SCENARIO_TO_POS(_WARP) - No scenario found within range", CTheScripts::GetCurrentScriptNameAndProgramCounter());
        #endif // __ASSERT
        return NULL;
      } else {
        bCheckPeds = true;
      }
    }

    if (bCheckPeds) {
      // Really check if the ped is actually using the scenario...
      // The CScenarioPoint.m_iUses won't get reset until the cleanup function is called in CTaskUseScenario
      // Other states here may be valid for us to start the scenario
      CPed::Pool* pool = CPed::GetPool();
      const int maxPeds = pool->GetSize();
      for (int i = 0; i < maxPeds; i++) {
        CPed* pCurPed = pool->GetSlot(i);
        if (!pCurPed || pCurPed == args.m_PedOrDummyToUsePoint || pCurPed->PopTypeIsMission()) {
          continue;
        }

        CTaskUseScenario* pTaskUseScenario = static_cast<CTaskUseScenario*>(pCurPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
        if (pTaskUseScenario) {
          if (pTaskUseScenario->GetScenarioPoint() != pScenarioPoint) {
            continue;
          }

          const int taskState = pTaskUseScenario->GetState();
          if (taskState != CTaskUseScenario::State_Start && taskState != CTaskUseScenario::State_GoToStartPosition) {
            #if __ASSERT
                                                                                                                                    if( !bCalledFromTask )
							scriptAssertf(0, "%s:TASK_USE_NEAREST_SCENARIO_TO_POS(_WARP) - No scenario found within range -- Scenario State For Another Ped Is Actively Using The Scenario", CTheScripts::GetCurrentScriptNameAndProgramCounter());
            #endif // __ASSERT
            return NULL;
          } else if (pCurPed->PopTypeIsRandom()) {
            // Scripted peds take priority over ambient/scenario peds
            // This will prevent undesirable conflicts at atms, vending machines, etc
            pTaskUseScenario->MakeAbortable(aiTask::ABORT_PRIORITY_URGENT, NULL);
          }
        }
      }
    }

    s32 scenarioType = CScenarioManager::ChooseRealScenario(pScenarioPoint->GetScenarioTypeVirtualOrReal(), pPed);

    bool bStartInCurrentPos = !bWarp;
    float fHeading = 0.0f;
    Vector3 vPedPos;
    CTask* pScenarioTask = CScenarioManager::SetupScenarioAndTask(scenarioType, vPedPos, fHeading, *pScenarioPoint, 0, bStartInCurrentPos);

    #if AI_DEBUG_OUTPUT_ENABLED
                                                                                                                            CNetObjGame* pNetObjPed = (NetworkInterface::IsGameInProgress() && pPed) ? pPed->GetNetworkObject() : NULL;
		CAILogManager::GetLog().Log("CommonUseNearestScenarioToPos: [0x%p][%s], (%.2f,%.2f,%.2f), Range (%.2f), Warp (%d), ScenarioType %d, ScenarioPoint 0x%p (%.2f,%.2f,%.2f)\r\n",
			pPed, pNetObjPed ? pNetObjPed->GetLogName() : "unknown",
			svScenarioPlace.x, svScenarioPlace.y, svScenarioPlace.z, fMaxRange, bWarp,
			scenarioType, pScenarioPoint, pScenarioPoint->GetWorldPosition().GetXf(), pScenarioPoint->GetWorldPosition().GetYf(), pScenarioPoint->GetWorldPosition().GetZf());
    #endif

    if (pScenarioTask) {
      if (pScenarioTask->GetTaskType() == CTaskTypes::TASK_USE_SCENARIO) {
        if (iTimeToLeave < 0) {
          ((CTaskUseScenario*) pScenarioTask)->SetDontLeaveEver();
        }

        if (iTimeToLeave > 0) {
          float fTimeToLeave = (float) iTimeToLeave;
          ((CTaskUseScenario*) pScenarioTask)->SetTimeToLeave(fTimeToLeave / 1000.0f);
        }
      }

      // This is probably not strictly necessary, as CTaskUseScenario should warp us into
      // place anyway. But, may still be safest, in case the script gets the position of the ped
      // before the task has run, or if we're using a ragdoll.
      if (bWarp && pPed) {
        // If we are using ragdoll, the calls to SetPosition() and other functions wouldn't work
        // well. FlushImmediately() should take care of this by ending tasks and doing other things
        // to restore the ped to a normal state. See B* 333114: "Ignorable Assert - Error: false:
        // CPed::UpdatePhysicsFromEntity called while ragdoll active - fired after starting private dance".
        // Note: we could potentially do this in the non-ragdoll case too, but to start out with, that
        // seemed a bit risky.
        if (pPed->GetUsingRagdoll()) {
          pPed->GetPedIntelligence()->FlushImmediately(true);
        }

        pPed->SetPosition(vPedPos);
        pPed->SetHeading(fHeading);
        pPed->SetDesiredHeading(fHeading);
      }
    } else {
      #if __ASSERT
                                                                                                                              if( !bCalledFromTask )
				scriptAssertf(0, "%s:TASK_USE_NEAREST_SCENARIO_TO_POS(_WARP) - scenario type found of invalid type", CTheScripts::GetCurrentScriptNameAndProgramCounter());
      #endif // __ASSERT
    }

    // We could probably do this if we wanted to wrap the scenario task in a CTaskUnalerted. The main
    // reason for that would probably be if we needed to support chaining when done using the current scenario.
    //	aiTask* pControlTask = CScenarioManager::SetupScenarioControlTask(*pScenarioInfo, pScenarioTask, *pPropScenarioPoint);

    return pScenarioTask;
  }

  CTask* CommonUseNearestScenarioChainToPos(CPed* pPed, const scrVector& svScenarioPlace, float fMaxRange, bool bWarp, bool bCalledFromTask, int iTimeToLeave) {
    CTask* pControlTask = NULL;
    if (SCRIPT_VERIFY(pPed, "Invalid ped in CommonUseNearestScenarioChainToPos!")) {
      // Construct CTaskUseScenario for the nearest scenario point.
      CTask* pScenarioTask = CommonUseNearestScenarioToPos(pPed, svScenarioPlace, fMaxRange, bWarp, bCalledFromTask, iTimeToLeave);

      if (SCRIPT_VERIFY(pScenarioTask, "Invalid scenario task!")) {
        CScenarioPoint* pScenarioPoint = pScenarioTask->GetScenarioPoint();
        if (SCRIPT_VERIFY(pScenarioPoint, "Generated scenario task did not have a valid scenario point!")) {
          s32 iScenarioType = CScenarioManager::ChooseRealScenario(pScenarioPoint->GetScenarioTypeVirtualOrReal(), pPed);
          const CScenarioInfo* pScenarioInfo = CScenarioManager::GetScenarioInfo(iScenarioType);
          if (SCRIPT_VERIFY(pScenarioInfo, "Invalid scenario info for type!")) {
            // Wrap the scenario point task in CTaskUnalerted so the ped can follow the chain.
            pControlTask = CScenarioManager::SetupScenarioControlTask(*pPed, *pScenarioInfo, pScenarioTask, pScenarioPoint, iScenarioType);
          }
        }
      }
    }
    return pControlTask;
  }

  void CommandTaskUseNearestScenarioToPos(int iPedIndex, const scrVector& svScenarioPlace, float fMaxRange, int iTimeToLeave) {
    // NULL peds are valid since this task can run in a sequence
    CPed* pPed = NULL;
    if (iPedIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    CTask* pTask = CommonUseNearestScenarioToPos(pPed, svScenarioPlace, fMaxRange, false, false, iTimeToLeave);
    if (pTask)
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_USE_NEAREST_SCENARIO_TO_POS, "TASK_USE_NEAREST_SCENARIO_TO_COORD");
  }

  void CommandTaskUseNearestScenarioToPosWarp(int iPedIndex, const scrVector& svScenarioPlace, float fMaxRange, int iTimeToLeave) {
    // NULL peds are valid since this task can run in a sequence
    CPed* pPed = NULL;
    if (iPedIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    CTask* pTask = CommonUseNearestScenarioToPos(pPed, svScenarioPlace, fMaxRange, true, false, iTimeToLeave);
    if (pTask)
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_USE_NEAREST_SCENARIO_TO_POS, "TASK_USE_NEAREST_SCENARIO_TO_COORD_WARP");
  }

  void CommandTaskUseNearestTrainScenarioToPosWarp(int iPedIndex, const scrVector& svScenarioPlace, float fMaxRange) {
    // NULL peds are valid since this task can run in a sequence
    CPed* pPed = NULL;
    if (iPedIndex != NULL_IN_SCRIPTING_LANGUAGE) {
      pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    }

    CTask* pUseScenarioTask = CommonUseNearestScenarioToPos(pPed, svScenarioPlace, fMaxRange, true, false, 0, true);
    CTaskRideTrain* pTaskRideTrain = NULL;

    if (pUseScenarioTask) {
      const CScenarioPoint* pScenarioPoint = pUseScenarioTask->GetScenarioPoint();

      if (pScenarioPoint) {
        //Check if the scenario is on a train.
        CEntity* pEntity = pScenarioPoint->GetEntity();
        if (pEntity && pEntity->GetIsTypeVehicle()) {
          CVehicle* pVehicle = static_cast<CVehicle*>(pEntity);
          if (pVehicle->InheritsFromTrain()) {
            //Grab the train.
            CTrain* pTrain = static_cast<CTrain*>(pVehicle);

            //Ride the train with the scenario point.
            pTaskRideTrain = rage_new
            CTaskRideTrain(pTrain, pUseScenarioTask);
          }
        }
      }
    }

    if (pTaskRideTrain) {
      CScriptPeds::GivePedScriptedTask(iPedIndex, pTaskRideTrain, SCRIPT_TASK_USE_NEAREST_TRAIN_SCENARIO_TO_POS, "TASK_USE_NEAREST_TRAIN_SCENARIO_TO_COORD_WARP");
    }
  }

  void CommandTaskUseNearestScenarioChainToPos(int iPedIndex, const scrVector& svScenarioInPlace, float fMaxRange, int iTimeToLeave) {
    if (SCRIPT_VERIFY(iPedIndex != NULL_IN_SCRIPTING_LANGUAGE, "Ped index was NULL!")) {
      // Find the ped.
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (pPed) {
        CTask* pTask = CommonUseNearestScenarioChainToPos(pPed, svScenarioInPlace, fMaxRange, false, false, iTimeToLeave);
        if (pTask) {
          // Give the ped the scenario following task.
          CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_USE_NEAREST_SCENARIO_CHAIN_TO_POS, "TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD");
        }
      }
    }
  }

  void CommandTaskUseNearestScenarioChainToPosWarp(int iPedIndex, const scrVector& svScenarioPlace, float fMaxRange, int iTimeToLeave) {
    if (SCRIPT_VERIFY(iPedIndex != NULL_IN_SCRIPTING_LANGUAGE, "Ped index was NULL!")) {
      // Find the ped.
      CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
      if (pPed) {
        CTask* pTask = CommonUseNearestScenarioChainToPos(pPed, svScenarioPlace, fMaxRange, true, false, iTimeToLeave);
        if (pTask) {
          // Give the ped the scenario following task.
          CScriptPeds::GivePedScriptedTask(iPedIndex, pTask, SCRIPT_TASK_USE_NEAREST_SCENARIO_CHAIN_TO_POS, "TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD_WARP");
        }
      }
    }
  }

  bool CommandIsPedActiveInScenario(int iPedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);
    if (SCRIPT_VERIFY(pPed, "PedIndex was invalid for IS_PED_ACTIVE_IN_SCENARIO or IS_PED_PLAYING_BASE_CLIP_IN_SCENARIO!")) {
      CTaskUseScenario* pTaskScenario = static_cast<CTaskUseScenario*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_USE_SCENARIO));
      if (pTaskScenario) {
        // The ped is really only "in" the scenario when they are playing the ambient animations associated with it.
        return pTaskScenario->GetState() == CTaskUseScenario::State_PlayAmbients;
      }
    }

    return false;
  }

  bool CommandIsPedPlayingBaseClipInScenario(int iPedIndex) {
    if (CommandIsPedActiveInScenario(iPedIndex)) {
      const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex);
      if (pPed) {
        CTaskAmbientClips* pTaskAmbientClips = static_cast<CTaskAmbientClips*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AMBIENT_CLIPS));
        if (pTaskAmbientClips) {
          return pTaskAmbientClips->IsBaseClipFullyBlended();
        }
      }
    }

    return false;
  }

  void CommandSetCanPlayAmbientIdles(int PedIndex, bool bBlockIdleClips, bool bRemoveIdleClipIfPlaying) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      CTaskAmbientClips* pTaskAmbientClips = static_cast<CTaskAmbientClips*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_AMBIENT_CLIPS));
      if (pTaskAmbientClips) {
        pTaskAmbientClips->SetBlockAmbientIdles(bBlockIdleClips);
        pTaskAmbientClips->SetRemoveAmbientIdles(bRemoveIdleClipIfPlaying);
      }
    }
  }

  void CommandTaskSwapWeapon(int PedIndex, bool bDrawWeapon) {
    u32 nFlags = SWAP_HOLSTER;
    if (bDrawWeapon)
      nFlags |= SWAP_DRAW;

    CTask* pTask = rage_new
    CTaskSwapWeapon(nFlags);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SWAP_WEAPON, "TASK_SWAP_WEAPON");
  }

  void CommandTaskReloadWeapon(int PedIndex, bool UNUSED_PARAM(bDrawWeapon)) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);

    if (SCRIPT_VERIFY(pPed, "PedIndex was invalid for TASK_RELOAD_WEAPON!") &&
        SCRIPT_VERIFY(pPed->GetWeaponManager(), "WeaponManager was invalid for TASK_RELOAD_WEAPON!") &&
        SCRIPT_VERIFY(pPed->GetWeaponManager()->GetEquippedWeapon(), "Equipped weapon was invalid for TASK_RELOAD_WEAPON!")) {
      CTaskReloadGun* pTaskToGiveToPed = rage_new
      CTaskReloadGun(CWeaponController::WCT_Reload);
      if (SCRIPT_VERIFY(pTaskToGiveToPed, "TASK_RELOAD_WEAPON - Unable to create reload gun task")) {
        CScriptPeds::GivePedScriptedTask(PedIndex, pTaskToGiveToPed, SCRIPT_TASK_RELOAD_WEAPON, "TASK_RELOAD_WEAPON");
      }
    }
  }

  void CommandTaskCombatRoll(int UNUSED_PARAM(PedIndex), s32 UNUSED_PARAM(iDirection)) {
    scriptAssertf(0, "TASK_COMBAT_ROLL removed");
  }

  void CommandTaskSkyDive(int iPedIndex, bool bInstant) {
    fwFlags32 fallFlags = FF_ForceSkyDive;
    if (bInstant) {
      fallFlags.SetFlag(FF_InstantBlend);
      fallFlags.SetFlag(FF_DisableSkydiveTransition);
    }

    CTaskComplexControlMovement* pCtrlTask;
    pCtrlTask = rage_new
    CTaskComplexControlMovement(rage_new
    CTaskMoveInAir(), rage_new
    CTaskFall(fallFlags), CTaskComplexControlMovement::TerminateOnSubtask );

    CScriptPeds::GivePedScriptedTask(iPedIndex, pCtrlTask, SCRIPT_TASK_SKY_DIVE, "TASK_SKY_DIVE");
  }

  void CommandTaskParachute(int iPedID, bool UNUSED_PARAM(bGiveParachuteItem), bool bInstant) {
    //Note: The 'give parachute' param is being ignored -- it is
    //		too error prone, and causes asserts and failed tasks.

    fwFlags32 paraFlags = CTaskParachute::PF_GiveParachute;
    if (bInstant) {
      paraFlags.SetFlag(CTaskParachute::PF_InstantBlend);
    }

    CTaskParachute* pTask = rage_new
    CTaskParachute(paraFlags);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PARACHUTE, "TASK_PARACHUTE");
  }

  void CommandTaskParachuteToTarget(int iPedID, const scrVector& svTargetLocation) {
    CTaskParachute* pTask = rage_new
    CTaskParachute(CTaskParachute::PF_GiveParachute | CTaskParachute::PF_SkipSkydiving);
    pTask->SetTargetPosition(VECTOR3_TO_VEC3V((Vector3) svTargetLocation));
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_PARACHUTE_TO_TARGET, "TASK_PARACHUTE_TO_TARGET");
  }

  void CommandSetParachuteTaskTarget(int iPedID, const scrVector& svTargetLocation) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      CTaskParachute* pTaskParachute = (CTaskParachute*) pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_PARACHUTE);
      if (SCRIPT_VERIFY(pTaskParachute, "Ped isn't running parachute task, please use GET_PED_PARACHUTE_STATE != PPS_INVALID to check")) {
        pTaskParachute->GetParachuteFlags().SetFlag(CTaskParachute::PF_SkipSkydiving);
        pTaskParachute->SetTargetPosition(VECTOR3_TO_VEC3V((Vector3) svTargetLocation));
      }
    }
  }

  void CommandSetParachuteTaskThrust(int iPedID, float fThrust) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      CTaskParachute* pTaskParachute = (CTaskParachute*) pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_PARACHUTE);
      if (SCRIPT_VERIFY(pTaskParachute, "Ped isn't running parachute task, please use GET_PED_PARACHUTE_STATE != PPS_INVALID to check")) {
        pTaskParachute->SetParachuteThrust(fThrust);
      }
    }
  }

  void CommandSetParachuteTaskTargetEntity(int iPedID, int iTargetID, const scrVector& vOffset) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      const CPhysical* pTarget = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(iTargetID);
      if (pTarget) {
        CTaskParachute* pTaskParachute = static_cast<CTaskParachute*>(pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_PARACHUTE));
        if (SCRIPT_VERIFY(pTaskParachute, "Ped isn't running parachute task, please use GET_PED_PARACHUTE_STATE != PPS_INVALID to check")) {
          pTaskParachute->SetTarget(pTarget, VECTOR3_TO_VEC3V((Vector3) vOffset));
        }
      }
    }
  }

  void CommandTaskJetpack(int iPedID) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      //Ensure the inventory is valid.
      CPedInventory* pPedInventory = pPed->GetInventory();
      if (pPedInventory) {
        //! Give ped a jetpack.
        pPedInventory->AddWeapon(GADGETTYPE_JETPACK);

        //! Equip it.
        pPed->SetPedConfigFlag(CPED_CONFIG_FLAG_EquipJetpack, true);
      }
    }
  }

  void CommandTaskJetpackGoToCoord(int JETPACK_ONLY(iPedID), const scrVector& JETPACK_ONLY(scrVecCoors), float JETPACK_ONLY(fMinHeightFromGround), bool JETPACK_ONLY(bDisableThrustAtDestination)) {
    #if ENABLE_JETPACK
                                                                                                                            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
		if(pPed)
		{
			CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_JETPACK);
			if (SCRIPT_VERIFY(pTask, "TASK_JETPACK_GO_TO_COORD - Not running jetpack task!"))
			{
				CTaskJetpack *pTaskJetpack = static_cast<CTaskJetpack*>(pTask);
				if (SCRIPT_VERIFY(pTaskJetpack, "TASK_JETPACK_GO_TO_COORD - Not running jetpack task!"))
				{
					Vector3 vCoord = Vector3(scrVecCoors);
					pTaskJetpack->SetGoToParametersForAI(NULL, vCoord, fMinHeightFromGround, bDisableThrustAtDestination);
				}
			}
		}
    #endif
  }

  void CommandTaskJetpackGoToEntity(int JETPACK_ONLY(iPedID), int JETPACK_ONLY(iTargetEntityID), const scrVector& JETPACK_ONLY(scrVecCoorsOffset), float JETPACK_ONLY(fMinHeightFromGround), bool JETPACK_ONLY(bDisableThrustAtDestination), bool JETPACK_ONLY(bUseRandomTimerWhenClose)) {
    #if ENABLE_JETPACK
                                                                                                                            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
		if(pPed)
		{
			CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_JETPACK);
			if (SCRIPT_VERIFY(pTask, "TASK_JETPACK_GO_TO_ENTITY - Not running jetpack task!"))
			{
				CTaskJetpack *pTaskJetpack = static_cast<CTaskJetpack*>(pTask);
				if (SCRIPT_VERIFY(pTaskJetpack, "TASK_JETPACK_GO_TO_ENTITY - Not running jetpack task!"))
				{
					CPhysical* pTargetPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(iTargetEntityID);
					if (SCRIPT_VERIFY(pTargetPhysical, "TASK_JETPACK_GO_TO_ENTITY - Target entity is not valid!"))
					{
						Vector3 vCoord = Vector3(scrVecCoorsOffset);
						pTaskJetpack->SetGoToParametersForAI(pTargetPhysical, vCoord, fMinHeightFromGround, bDisableThrustAtDestination, bUseRandomTimerWhenClose);
					}
				}
			}
		}
    #endif
  }

  void CommandTaskJetpackShootAtCoord(int JETPACK_ONLY(iPedID), const scrVector& JETPACK_ONLY(scrVecCoors), float JETPACK_ONLY(fAbortRange), int JETPACK_ONLY(iFrequencyPercentage), int JETPACK_ONLY(iFiringPatternHash)) {
    #if ENABLE_JETPACK
                                                                                                                            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
		if(pPed)
		{
			CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_JETPACK);
			if (SCRIPT_VERIFY(pTask, "TASK_JETPACK_SHOOT_AT_COORD - Not running jetpack task!"))
			{
				CTaskJetpack *pTaskJetpack = static_cast<CTaskJetpack*>(pTask);
				if (SCRIPT_VERIFY(pTaskJetpack, "TASK_JETPACK_SHOOT_AT_COORD - Not running jetpack task!"))
				{
					Vector3 vCoord = Vector3(scrVecCoors);
					pTaskJetpack->SetShootAtParametersForAI(NULL, vCoord, fAbortRange, iFrequencyPercentage, iFiringPatternHash);
				}
			}
		}
    #endif
  }

  void CommandTaskJetpackShootAtEntity(int JETPACK_ONLY(iPedID), int JETPACK_ONLY(iTargetEntityID), const scrVector& JETPACK_ONLY(scrVecCoorsOffset), float JETPACK_ONLY(fAbortRange), int JETPACK_ONLY(iFrequencyPercentage), int JETPACK_ONLY(iFiringPatternHash)) {
    #if ENABLE_JETPACK
                                                                                                                            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
		if(pPed)
		{
			CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_JETPACK);
			if (SCRIPT_VERIFY(pTask, "TASK_JETPACK_SHOOT_AT_ENTITY - Not running jetpack task!"))
			{
				CTaskJetpack *pTaskJetpack = static_cast<CTaskJetpack*>(pTask);
				if (SCRIPT_VERIFY(pTaskJetpack, "TASK_JETPACK_SHOOT_AT_ENTITY - Not running jetpack task!"))
				{
					Vector3 vCoord = Vector3(scrVecCoorsOffset);

					CPhysical* pTargetPhysical = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(iTargetEntityID);
					if (SCRIPT_VERIFY(pTargetPhysical, "TASK_JETPACK_SHOOT_AT_ENTITY - Target entity is not valid!"))
					{
						pTaskJetpack->SetShootAtParametersForAI(pTargetPhysical, vCoord, fAbortRange, iFrequencyPercentage, iFiringPatternHash);
					}

				}
			}
		}
    #endif
  }

  void CommandTaskJetpackStopShooting(int JETPACK_ONLY(iPedID)) {
    #if ENABLE_JETPACK
                                                                                                                            CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
		if(pPed)
		{
			CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_JETPACK);
			if (SCRIPT_VERIFY(pTask, "TASK_JETPACK_SHOOT_AT_COORD - Not running jetpack task!"))
			{
				CTaskJetpack *pTaskJetpack = static_cast<CTaskJetpack*>(pTask);
				if (SCRIPT_VERIFY(pTaskJetpack, "TASK_JETPACK_SHOOT_AT_COORD - Not running jetpack task!"))
				{
					pTaskJetpack->RequestDrivebyTermination(true);
				}
			}
		}
    #endif
  }

  void CommandTaskRappelFromHeli(int iPedIndex, float fMinRappelHeight) {
    CTaskHeliPassengerRappel* pRappelTask = rage_new
    CTaskHeliPassengerRappel(fMinRappelHeight);
    CScriptPeds::GivePedScriptedTask(iPedIndex, pRappelTask, SCRIPT_TASK_RAPPEL_FROM_HELI, "TASK_RAPPEL_FROM_HELI");
  }

  void CommandTaskDragPedToCoord(int iDraggerIndex, int iDraggedIndex, const scrVector& vCoords, float fMoveBlendRatioForApproach) {
    CPed* pDragged = CTheScripts::GetEntityToModifyFromGUID<CPed>(iDraggedIndex, CTheScripts::GUID_ASSERT_FLAGS_NO_DEAD_CHECK);
    if (pDragged) {
      CTaskDraggingToSafety* pTask = rage_new
      CTaskDraggingToSafety(pDragged, NULL, CTaskDraggingToSafety::CF_CoverNotRequired);
      pTask->DragToPosition(vCoords);
      pTask->SetMoveBlendRatioForApproach(fMoveBlendRatioForApproach);

      CScriptPeds::GivePedScriptedTask(iDraggerIndex, pTask, SCRIPT_TASK_DRAG_PED_TO_COORD, "TASK_DRAG_PED_TO_COORD");
    }
  }

  bool CommandIsPedGettingUp(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetPedIntelligence()->IsPedGettingUp();
    }
    return false;
  }

  void CommandTaskWrithe(int iPedID, int iPedTargetID, int nMinFireLoops, int UNUSED_PARAM(StartState), bool bForceShootOnGround, int nShootFromGroundTimer) {
    const CPed* pTargetPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedTargetID);
    if (SCRIPT_VERIFY(pTargetPed, "SCRIPT_TASK_WRITHE - Invalid target")) {
      bool bFromGetUp = true;
      bool bEnableRagdollOnCollision = true;

      if (bForceShootOnGround) {
        bEnableRagdollOnCollision = false;
      }

      CTaskWrithe* pTask = rage_new
      CTaskWrithe(CWeaponTarget(pTargetPed), bFromGetUp, bEnableRagdollOnCollision, bForceShootOnGround);
      pTask->SetMinFireLoops(nMinFireLoops);
      pTask->SetShootFromGroundTimer(nShootFromGroundTimer);

      CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_WRITHE, "TASK_WRITHE");
    }
  }

  bool CommandIsPedInWrithe(int iPedID) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedID);
    if (SCRIPT_VERIFY(pPed, "SCRIPT_TASK_WRITHE - Invalid target")) {
      return pPed->GetPedResetFlag(CPED_RESET_FLAG_IsInWrithe);
    }

    return false;
  }

  void CommandOpenPatrolRoute(const char* RouteName) {
    CTheScripts::GetScriptPatrol().OpenPatrolRoute(RouteName);
  }

  void CommandClosePatrolRoute() {
    CTheScripts::GetScriptPatrol().ClosePatrolRoute();
  }

  void CommandAddPatrolNode(int NodeId, const char* NodeType, const scrVector& scrNodePos, const scrVector& scrNodeHeading, int Duration) {
    Vector3 vNodePos(scrNodePos);
    Vector3 vNodeHeading(scrNodeHeading);

    CTheScripts::GetScriptPatrol().AddPatrolNode(NodeId, NodeType, vNodePos, vNodeHeading, Duration);
  }

  void CommandAddPatrolNodeLink(int NodeID1, int NodeID2) {
    CTheScripts::GetScriptPatrol().AddPatrolLink(NodeID1, NodeID2);
  }

  void CommandCreatePatrolRoute() {
    CTheScripts::GetScriptPatrol().CreatePatrolRoute();
  }

  void CommandDeletePatrolRoute(const char* RouteName) {
    u32 HashedRouteName = atStringHash(RouteName);

    CTheScripts::GetCurrentGtaScriptHandler()->RemoveScriptResource(CGameScriptResource::SCRIPT_RESOURCE_PATROL_ROUTE, static_cast<ScriptResourceRef>(HashedRouteName));
  }

  void CommandTaskPatrol(int PedIndex, const char* cPatrolRouteName, int iAlertState, bool bCanChatToPeds, bool bUseHeadLookAt) {
    SCRIPT_ASSERT(cPatrolRouteName, "Error, NULL patrol route name, did you forget to initialise the route name?");

    int iRouteNameHash = atStringHash(cPatrolRouteName);
    CTaskPatrol::AlertStatus asAlertState = CTaskPatrol::AlertStatus(iAlertState);
    u32 iPatrolFlags = 0;
    if (bCanChatToPeds) {
      iPatrolFlags |= GF_CanChatToPeds;
    }
    if (bUseHeadLookAt) {
      iPatrolFlags |= GF_UseHeadLookAt;
    }

    CTask* pTask = rage_new
    CTaskPatrol(iRouteNameHash, asAlertState, iPatrolFlags);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_PATROL, "TASK_PATROL");
  }

  void CommandTaskStayInCover(int PedIndex) {
    CTask* pTask = rage_new
    CTaskStayInCover();
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_STAY_IN_COVER, "TASK_STAY_IN_COVER");
  }

  bool CommandGetPatrolNodeInfo(int PedIndex, int& iTimeLeftAtNode, int& iNodeId) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    bool bHaveVaildTask = false;

    if (pPed) {

      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_PATROL);
      //CTask* pTask = pPedAI->GetTaskManager()->GetActiveTask();

      if (pTask && pTask->GetTaskType() == CTaskTypes::TASK_PATROL) {
        CTaskPatrol* pPatrolTask = (CTaskPatrol*) pTask;
        iNodeId = pPatrolTask->CTaskPatrol::GetCurrentNode();
        iTimeLeftAtNode = pPatrolTask->CTaskPatrol::GetTimeLeftAtNode();
        bHaveVaildTask = true;
      }

    }
    return bHaveVaildTask;

  }

  void CommandTaskHangGlider(int UNUSED_PARAM(PedIndex), int UNUSED_PARAM(HangGliderObjectIndex)) {
    Assertf(0, "HangGlider is deprecated functionality");
  }

  void CommandSetHangGliderAirSpeed(int UNUSED_PARAM(PedIndex), const scrVector& UNUSED_PARAM(scrVAirSpeed)) {
    Assertf(0, "HangGlider is deprecated functionality");
  }

  void CommandVehicleShootAtPed(int PedIndex, int OtherPedIndex, float fFireTolerance) {
    const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(OtherPedIndex);

    if (pOtherPed) {
      CAITarget target(pOtherPed);
      CTask* pTask = rage_new
      CTaskVehicleCombat(&target, 0, fFireTolerance);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_VEHICLE_SHOOT_AT_ENTITY, "TASK_VEHICLE_SHOOT_AT_PED");
    }
  }

  void CommandVehicleAimAtPed(int PedIndex, int OtherPedIndex) {
    const CPed* pOtherPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(OtherPedIndex);

    if (pOtherPed) {
      CAITarget target(pOtherPed);
      CTask* pTask = rage_new
      CTaskVehicleCombat(&target, CTaskVehicleCombat::Flag_justAim);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_VEHICLE_AIM_AT_ENTITY, "TASK_VEHICLE_AIM_AT_PED");
    }
  }

  void CommandVehicleShootAtCoord(int PedIndex, const scrVector& svTarget, float fFireTolerance) {
    Vector3 vTarget(svTarget);
    CAITarget target(vTarget);

    CTask* pTask = rage_new
    CTaskVehicleCombat(&target, 0, fFireTolerance);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_VEHICLE_SHOOT_AT_COORD, "TASK_VEHICLE_SHOOT_AT_COORD");
  }

  void CommandVehicleAimAtCoord(int PedIndex, const scrVector& svTarget) {
    Vector3 vTarget(svTarget);
    CAITarget target(vTarget);

    CTask* pTask = rage_new
    CTaskVehicleCombat(&target, CTaskVehicleCombat::Flag_justAim);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_VEHICLE_AIM_AT_COORD, "TASK_VEHICLE_AIM_AT_COORD");
  }

  void CommandVehicleAimUsingCamera(int PedIndex) {
    CTask* pTask = rage_new
    CTaskVehicleCombat(NULL, CTaskVehicleCombat::Flag_useCamera);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_VEHICLE_AIM_USING_CAMERA, "TASK_VEHICLE_AIM_USING_CAMERA");
  }

  // Give a ped the bind-pose task to examine the transition from NM to animation.
  // DEV ONLY!
  void CommandTaskBindPose(int DEV_ONLY(iPedID), bool DEV_ONLY(bDoBlendFromNM)) {
    #if __DEV
                                                                                                                            CTask* pTask = rage_new CTaskBindPose(bDoBlendFromNM);
		CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_BIND_POSE, "TASK_BIND_POSE");
    #endif // __DEV
  }

  // Give ped an NM electrocution task.
  void CommandElectrocute(int iPedID, int nMinTime, int nMaxTime) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed && CTaskNMBehaviour::CanUseRagdoll(pPed, RAGDOLL_TRIGGER_SCRIPT)) {
      UpdatePedRagdollBoundsForScriptActivation(pPed);
      CTask* pNmTask = rage_new
      CTaskNMElectrocute(nMinTime, nMaxTime);
      CEventSwitch2NM event(nMaxTime, pNmTask, true, nMinTime);
      pPed->SwitchToRagdoll(event);
    }

  }

  // Give ped a high fall task.
  void CommandHighFall(int iPedID, int nMinTime, int nMaxTime, int eEntryType) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed && CTaskNMBehaviour::CanUseRagdoll(pPed, RAGDOLL_TRIGGER_SCRIPT)) {
      UpdatePedRagdollBoundsForScriptActivation(pPed);
      CTask* pNmTask = rage_new
      CTaskNMHighFall(nMinTime, NULL, (CTaskNMHighFall::eHighFallEntryType) eEntryType);
      CEventSwitch2NM event(nMaxTime, pNmTask, true, nMinTime);
      pPed->SwitchToRagdoll(event);
    }
  }

  // Give ped an NM dangle-from-meathook task.
  void CommandDangleFromMeathook(bool bStart, int iPedID, int iHookID, const scrVector& scrPos, bool bDoGrab, float fFrequency, int UNUSED_PARAM(nMinTime), int nMaxTime,
                                 bool bFixRotation, float fRotMinX, float fRotMinY, float fRotMinZ, float fRotMaxX, float fRotMaxY, float fRotMaxZ) {
    Vector3 pos = Vector3(scrPos.x, scrPos.y, scrPos.z);
    Vector3 vecRotMinLimits = Vector3(fRotMinX, fRotMinY, fRotMinZ);
    Vector3 vecRotMaxLimits = Vector3(fRotMaxX, fRotMaxY, fRotMaxZ);

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed) {
      // First time activating?
      if (bStart && CTaskNMBehaviour::CanUseRagdoll(pPed, RAGDOLL_TRIGGER_SCRIPT)) {
        UpdatePedRagdollBoundsForScriptActivation(pPed);

        // Give it the dangle task
        CTask* pNmTask = rage_new
        CTaskNMDangle(pos);
        CEventSwitch2NM event(nMaxTime, pNmTask, true);
        pPed->SwitchToRagdoll(event);

        // Set grab state
        ((CTaskNMDangle*) pNmTask)->SetGrabParams(pPed, bDoGrab, fFrequency);

        // Create the hook to world constraint
        if (pPed->GetRagdollConstraintData()) {
          CPhysical* pHook = CTheScripts::GetEntityToModifyFromGUID<CPhysical>(iHookID);
          AssertMsg(pHook && pHook->GetCurrentPhysicsInst(), "CommandDangleFromMeathook - valid hookID not provided");
          if (pHook && pHook->GetCurrentPhysicsInst())
            pPed->GetRagdollConstraintData()->CreateHookToWorldConstraint(pHook, pos, bFixRotation, vecRotMinLimits, vecRotMaxLimits);
        }
      }

      // Set grab state
      if (CTaskNMDangle * pTaskDangle = static_cast<CTaskNMDangle*>(
        pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_NM_DANGLE))) {
        pTaskDangle->SetGrabParams(pPed, bDoGrab, fFrequency);
        pTaskDangle->UpdateHookConstraint(pos, vecRotMinLimits, vecRotMaxLimits);
      } else if (pPed->GetRagdollConstraintData()) {
        // Update the hook to world constraint manually if the dangle task has ended
        pPed->GetRagdollConstraintData()->UpdateHookToWorldConstraint(pos, vecRotMinLimits, vecRotMaxLimits);
      }
    }
  }

  // Give ped an NM slung-over-shoulder task.
  void CommandSlungOverShoulder(int iPedIDSlungPed, int iPedIDCarrier, int nMinTime, int nMaxTime) {
    CPed* pPedSlung = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIDSlungPed);
    CPed* pPedCarrier = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIDCarrier);
    if (pPedSlung && pPedCarrier && CTaskNMBehaviour::CanUseRagdoll(pPedSlung, RAGDOLL_TRIGGER_SLUNG_OVER_SHOULDER)) {
      // Perform normal collision to avoid popping upon activation
      pPedSlung->GetRagdollInst()->SetDontZeroMatricesOnActivation();

      UpdatePedRagdollBoundsForScriptActivation(pPedSlung);

      // Give it the slung over shoulder task
      CTask* pNmTask = rage_new
      CTaskNMSlungOverShoulder(pPedSlung, pPedCarrier);
      CEventSwitch2NM event(nMaxTime, pNmTask, true, nMinTime);
      pPedSlung->SwitchToRagdoll(event);
    }
  }

  // Give ped an NM stumble task.
  void CommandStumble(int iPedID, int nMinTime, int nMaxTime) {
    Assertf(0, "CommandStumble is no longer supported");

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
    if (pPed && CTaskNMBehaviour::CanUseRagdoll(pPed, RAGDOLL_TRIGGER_SCRIPT)) {
      UpdatePedRagdollBoundsForScriptActivation(pPed);

      CTask* pNmTask = rage_new
      CTaskNMRelax(nMinTime, nMaxTime);
      CEventSwitch2NM event(nMaxTime, pNmTask, true, nMinTime);
      pPed->SwitchToRagdoll(event);
    }
  }

  void CommandTaskSetBlockingOfNonTemporaryEvents(int PedIndex, bool bSet) {
    CTask* pTask = rage_new
    CTaskSetBlockingOfNonTemporaryEvents(bSet);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS, "TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS");
  }

  void CommandTaskForceMotionState(int PedIndex, int motionState, bool bForceRestart) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex, 0);
    if (scriptVerifyf(CPed::IsAllowedToForceMotionState((CPedMotionStates::eMotionState) motionState, pPed), "%s: TASK_FORCE_MOTION_STATE - Failed to force state %d - See TTY for details", CTheScripts::GetCurrentScriptNameAndProgramCounter(), motionState)) {
      CTask* pTask = rage_new
      CTaskForceMotionState((CPedMotionStates::eMotionState) motionState, bForceRestart);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_FORCE_MOTION_STATE, "TASK_FORCE_MOTION_STATE");
    }
  }

  void CommandTaskMoveNetworkByName(int PedIndex, const char* network, float blendDuration, bool bAllowOverrideCloneUpdate, const char* szDictionary, s32 flags) {
    if (scriptVerifyf(g_NetworkDefStore.FindSlot(network) != -1, "TASK_MOVE_NETWORK_BY_NAME - Network def '%s' does not exist!", network)) {
      u32 iFlags = (u32) flags;

      if (bAllowOverrideCloneUpdate) {
        iFlags |= CTaskMoVEScripted::Flag_AllowOverrideCloneUpdate;
      }

      CTaskMoVEScripted* pTask = rage_new
      CTaskMoVEScripted(fwMvNetworkDefId(network), iFlags, blendDuration, VEC3_ZERO, Quaternion::sm_I);

      if (szDictionary && szDictionary[0] != '\0') {
        pTask->SetCloneSyncDictHash(atStringHash(szDictionary));
      }

      bool bPartOfASequence = (PedIndex == NULL_IN_SCRIPTING_LANGUAGE);

      if (iFlags & CTaskMoVEScripted::Flag_Secondary) {
        if (SCRIPT_VERIFY(!bPartOfASequence, "TASK_MOVE_NETWORK_BY_NAME_SECONDARY* Cant be used in a sequence!")) {
          CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
          if (pPed) {
            pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
          }
        }
        return;
      }

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_MOVE_NETWORK, "TASK_MOVE_NETWORK");
    }
  }

  void CommandTaskMoveNetworkByNameWithInitParams(int PedIndex, const char* network, int& initialParameters, float blendDuration, bool bAllowOverrideCloneUpdate, const char* szDictionary, s32 flags) {
    if (scriptVerifyf(g_NetworkDefStore.FindSlot(network) != -1, "TASK_MOVE_NETWORK_BY_NAME - Network def '%s' does not exist!", network)) {
      u32 iFlags = (u32) flags;

      if (bAllowOverrideCloneUpdate) {
        iFlags |= CTaskMoVEScripted::Flag_AllowOverrideCloneUpdate;
      }

      CTaskMoVEScripted::ScriptInitialParameters* pInitParams = reinterpret_cast<CTaskMoVEScripted::ScriptInitialParameters*>(&initialParameters);

      CTaskMoVEScripted* pTask = rage_new
      CTaskMoVEScripted(fwMvNetworkDefId(network), iFlags, blendDuration, VEC3_ZERO, Quaternion::sm_I, pInitParams);

      if (szDictionary && szDictionary[0] != '\0') {
        pTask->SetCloneSyncDictHash(atStringHash(szDictionary));
      }

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_MOVE_NETWORK, "TASK_MOVE_NETWORK");
    }
  }

  void CommandTaskMoveNetworkAdvancedByName(int PedIndex, const char* network, const scrVector& pos, const scrVector& rot, s32 RotOrder, float blendDuration, bool bAllowOverrideCloneUpdate, const char* szDictionary, s32 flags) {
    if (scriptVerifyf(g_NetworkDefStore.FindSlot(network) != -1, "TASK_MOVE_NETWORK_ADVANCED_BY_NAME - Network def '%s' does not exist!", network)) {
      u32 iFlags = ((u32) flags);

      iFlags |= CTaskMoVEScripted::Flag_SetInitialPosition;

      if (bAllowOverrideCloneUpdate) {
        iFlags |= CTaskMoVEScripted::Flag_AllowOverrideCloneUpdate;
      }

      Vector3 vPos(pos);
      Vector3 vRot(rot);

      vRot *= DtoR;

      Quaternion qRot;
      CScriptEulers::QuaternionFromEulers(qRot, vRot, static_cast<EulerAngleOrder>(RotOrder));
      CTaskMoVEScripted* pTask = rage_new
      CTaskMoVEScripted(fwMvNetworkDefId(network), iFlags, blendDuration, vPos, qRot);

      if (szDictionary && szDictionary[0] != '\0') {
        pTask->SetCloneSyncDictHash(atStringHash(szDictionary));
      }

      bool bPartOfASequence = (PedIndex == NULL_IN_SCRIPTING_LANGUAGE);

      if (iFlags & CTaskMoVEScripted::Flag_Secondary) {
        if (SCRIPT_VERIFY(!bPartOfASequence, "TASK_MOVE_NETWORK_ADVANCED_BY_NAME_SECONDARY* Cant be used in a sequence!")) {
          CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
          if (pPed) {
            pPed->GetPedIntelligence()->AddTaskSecondary(pTask, PED_TASK_SECONDARY_PARTIAL_ANIM);
          }
        }
        return;
      }

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_MOVE_NETWORK, "TASK_MOVE_NETWORK_ADVANCED");
    }
  }

  CTaskMoVEScripted* FindScriptedMoveTask(const CPed* pPed) {
    CTask* pTask = pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_MOVE_SCRIPTED);

    if (!pTask) {
      pTask = pPed->GetPedIntelligence()->FindTaskSecondaryByType(CTaskTypes::TASK_MOVE_SCRIPTED);
    }

    return static_cast<CTaskMoVEScripted*>(pTask);
  }

  void CommandTaskMoveNetworkAdvancedByNameWithInitParams(int PedIndex, const char* network, int& initialParameters, const scrVector& pos, const scrVector& rot, s32 RotOrder, float blendDuration, bool bAllowOverrideCloneUpdate, const char* szDictionary, s32 flags) {
    if (scriptVerifyf(g_NetworkDefStore.FindSlot(network) != -1, "TASK_MOVE_NETWORK_ADVANCED_BY_NAME - Network def '%s' does not exist!", network)) {
      u32 iFlags = ((u32) flags);

      iFlags |= CTaskMoVEScripted::Flag_SetInitialPosition;

      if (bAllowOverrideCloneUpdate) {
        iFlags |= CTaskMoVEScripted::Flag_AllowOverrideCloneUpdate;
      }

      CTaskMoVEScripted::ScriptInitialParameters* pInitParams = reinterpret_cast<CTaskMoVEScripted::ScriptInitialParameters*>(&initialParameters);

      Vector3 vPos(pos);
      Vector3 vRot(rot);

      vRot *= DtoR;

      Quaternion qRot;
      CScriptEulers::QuaternionFromEulers(qRot, vRot, static_cast<EulerAngleOrder>(RotOrder));
      CTaskMoVEScripted* pTask = rage_new
      CTaskMoVEScripted(fwMvNetworkDefId(network), iFlags, blendDuration, vPos, qRot, pInitParams);

      if (szDictionary && szDictionary[0] != '\0') {
        pTask->SetCloneSyncDictHash(atStringHash(szDictionary));
      }

      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_MOVE_NETWORK, "TASK_MOVE_NETWORK_ADVANCED");
    }
  }

  bool CommandIsTaskMoveNetworkActive(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (pTaskMoVE && pTaskMoVE->GetIsNetworkActive()) {
        return true;
      }
    }
    return false;
  }

  void CommandSetTaskMoveNetworkClipSet(s32 pedIndex, s32 clipSetHash, s32 variableClipSetHash = 0) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(pedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_CLIP_SET - task not running! Have you called TASK_MOVE_NETWORK_BY_NAME yet?")) {
        fwMvClipSetVarId varId;
        fwMvClipSetId setId;
        if (variableClipSetHash == 0) {
          variableClipSetHash = CLIP_SET_VAR_ID_INVALID;
        }

        varId.SetHash((u32) variableClipSetHash);
        setId.SetHash((u32) clipSetHash);

        pTaskMoVE->SetClipSet(varId, setId);
      }
    }
  }

  bool CommandIsTaskMoveNetworkReadyForTransition(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->GetState() == CTaskMoVEScripted::State_InState;
        }
      }
    }
    return false;
  }

  void CommandRequestTaskMoveNetworkStateTransition(int PedIndex, const char* szState) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          if (SCRIPT_VERIFY(pTaskMoVE->GetState() == CTaskMoVEScripted::State_InState, "REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION - task not ready for a transition! Check IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION!")) {
            pTaskMoVE->RequestStateTransition(szState);
          }
        }
      }
    }
  }

  void CommandSetExpectedCloneNextTaskMoveNetworkState(int PedIndex, const char* szState) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          if (SCRIPT_VERIFY(pTaskMoVE->GetState() == CTaskMoVEScripted::State_InState, "SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE - task not ready for a transition! Check IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION!")) {
            pTaskMoVE->SetExpectedCloneNextStateTransition(szState);
          }
        }
      }
    }
  }

  const char* CommandGetTaskMoveNetworkState(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "GET_TASK_MOVE_NETWORK_STATE - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "GET_TASK_MOVE_NETWORK_STATE - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->GetScriptStateName();
        }
      }
    }
    return "Unknown";
  }

  void CommandSetTaskMoveNetworkSignalFloat(int PedIndex, const char* szSignal, float fSignal) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          if (SCRIPT_VERIFY((!pPed->IsNetworkClone() || pTaskMoVE->GetAllowOverrideCloneUpdate()),
                            "SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT - Can't set float on cloned ped if its task is not being overridden!")) {
            pTaskMoVE->SetSignalFloat(szSignal, fSignal);
          }
        }
      }
    }
  }

  void CommandSetTaskMoveNetworkSignalFloatLerpRate(int PedIndex, const char* szSignal, float fLerpRate) {
    scriptAssertf(fLerpRate != 0.0f, "CommandSetTaskMoveNetworkSignalFloatLerpRate don't expect fLerpRate to be 0.0f");

    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT_LERP_RATE - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT_LERP_RATE - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->SetSignalFloatLerpRate(szSignal, fLerpRate);
        }
      }
    }
  }

  float CommandGetTaskMoveNetworkSignalFloat(int PedIndex, const char* szSignal) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "GET_TASK_MOVE_NETWORK_SIGNAL_FLOAT - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "GET_TASK_MOVE_NETWORK_SIGNAL_FLOAT - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->GetSignalFloat(szSignal);
        }
      }
    }
    return 0.0f;
  }

  void CommandSetTaskMoveNetworkSignalBool(int PedIndex, const char* szSignal, bool bSignal) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_SIGNAL_BOOL - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_SIGNAL_BOOL - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          if (SCRIPT_VERIFY((!pPed->IsNetworkClone() || pTaskMoVE->GetAllowOverrideCloneUpdate()),
                            "SET_TASK_MOVE_NETWORK_SIGNAL_BOOL - Can't set bool on cloned ped if its task is not being overridden!")) {
            pTaskMoVE->SetSignalBool(szSignal, bSignal);
          }
        }
      }
    }
  }

  void CommandSetTaskMoveNetworkSignalLocalBool(int PedIndex, const char* szSignal, bool bSignal) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_BOOL - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_BOOL - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          pTaskMoVE->SetSignalBool(szSignal, bSignal, false);
        }
      }
    }
  }

  void CommandSetTaskMoveNetworkSignalLocalFloat(int PedIndex, const char* szSignal, float signal) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_FLOAT - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_FLOAT - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          pTaskMoVE->SetSignalFloat(szSignal, signal, false);
        }
      }
    }
  }

  bool CommandGetTaskMoveNetworkSignalBool(int PedIndex, const char* szSignal) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "GET_TASK_MOVE_NETWORK_SIGNAL_BOOL - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE->GetIsNetworkActive(), "GET_TASK_MOVE_NETWORK_SIGNAL_BOOL - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->GetSignalBool(szSignal);
        }
      }
    }
    return false;
  }

  bool CommandGetTaskMoveNetworkEvent(int PedIndex, const char* eventName) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "GET_TASK_MOVE_NETWORK_EVENT - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE && pTaskMoVE->GetIsNetworkActive(), "GET_TASK_MOVE_NETWORK_EVENT - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->GetMoveEvent(eventName);
        }
      }
    }
    return false;
  }

  void CommandSetTaskMoveNetworkEnableCollisionOnNetworkCloneWhenFixed(int PedIndex, bool bEnableCollisionOnNetworkCloneWhenFixed) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed && pPed->GetPedIntelligence()) {
      CTaskMoVEScripted* pTaskMoVE = FindScriptedMoveTask(pPed);
      if (SCRIPT_VERIFY(pTaskMoVE, "SET_TASK_MOVE_NETWORK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED - task is not running!")) {
        if (SCRIPT_VERIFY(pTaskMoVE && pTaskMoVE->GetIsNetworkActive(), "SET_TASK_MOVE_NETWORK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED - task not active! Check IS_TASK_MOVE_NETWORK_ACTIVE!")) {
          return pTaskMoVE->SetEnableCollisionOnNetworkCloneWhenFixed(bEnableCollisionOnNetworkCloneWhenFixed);
        }
      }
    }
  }

  void PlayVehicleAnimScriptCommand(int VehicleIndex, const char* pAnimName, const char* pAnimDictNameStr) {
    const strStreamingObjectName pAnimDictName(pAnimDictNameStr);

    if (!SCRIPT_VERIFY(pAnimName, "VEHICLE_TASK_PLAY_ANIM - Animation name is null")) {
      return;
    }

    if (!SCRIPT_VERIFY(pAnimDictName.IsNotNull(), "VEHICLE_TASK_PLAY_ANIM - Animation dictionary name is null")) {
      return;
    }

    CVehicle* pVehicle = NULL;
    if (NULL_IN_SCRIPTING_LANGUAGE != VehicleIndex) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(VehicleIndex);
      if (!pVehicle) {
        return;
      }
    }

    //scriptAssertf(strlen(pAnimName)<ANIM_NAMELEN, "%s:TASK_PLAY_ANIM - Animation name is null : %s", CTheScripts::GetCurrentScriptNameAndProgramCounter(), pAnimName);


    aiTask* pTask = rage_new
    CTaskVehicleAnimation(pAnimDictName, pAnimName);

    if (pVehicle) {
      pVehicle->GetIntelligence()->AddTask(VEHICLE_TASK_TREE_SECONDARY, pTask, VEHICLE_TASK_SECONDARY_ANIM);
    }
  }

  void CommandVehicleTaskPlayAnim(int VehicleIndex, const char* pAnimDictName, const char* pAnimName) {
    PlayVehicleAnimScriptCommand(VehicleIndex, pAnimName, pAnimDictName);
  }

  bool CommandIsMoveBlendRatioStill(float fMoveBlendRatio) {
    return CPedMotionData::GetIsStill(fMoveBlendRatio);
  }

  bool CommandIsMoveBlendRatioWalking(float fMoveBlendRatio) {
    return CPedMotionData::GetIsWalking(fMoveBlendRatio);
  }

  bool CommandIsMoveBlendRatioRunning(float fMoveBlendRatio) {
    return CPedMotionData::GetIsRunning(fMoveBlendRatio);
  }

  bool CommandIsMoveBlendRatioSprinting(float fMoveBlendRatio) {
    return CPedMotionData::GetIsSprinting(fMoveBlendRatio);
  }

  bool CommandIsPedStill(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetMotionData()->GetIsStill();
    }
    return false;
  }

  bool CommandIsPedWalking(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetMotionData()->GetIsWalking();
    }
    return false;
  }

  bool CommandPedIsRunning(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetMotionData()->GetIsRunning();
    }
    return false;
  }

  bool CommandPedIsSprinting(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetMotionData()->GetIsSprinting();
    }
    return false;
  }

  bool CommandPedIsStrafing(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    if (pPed) {
      return pPed->GetMotionData()->GetIsStrafing();
    }
    return false;
  }

  /////////////////////////////////////////////////
  // Start the sweep task and point at an entity
  /////////////////////////////////////////////////
  void CommandTaskSweepAimEntity(int PedIndex, const char* pAnimDictName, const char* pLowAnimName, const char* pMedAnimName, const char* pHiAnimName, int runTime, int TargetEntityIndex, float turnRate, float fBlendInDuration) {
    const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(TargetEntityIndex);
    if (pEntity) {
      int clipDictId = fwAnimManager::FindSlot(strStreamingObjectName(pAnimDictName)).Get();
      fwMvClipId lowClipId(pLowAnimName);
      fwMvClipId medClipId(pMedAnimName);
      fwMvClipId highClipId(pHiAnimName);

      aiTask* pTask = rage_new
      CTaskGeneralSweep(clipDictId, lowClipId, medClipId, highClipId, runTime, pEntity, turnRate, fBlendInDuration);
      CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GENERAL_SWEEP, "TASK_GENERAL_SWEEP");
    }
  }

  // Update the sweep task and point at an entity
  void CommandUpdateTaskSweepAimEntity(int PedIndex, int TargetEntityIndex) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      // Check if the task is running
      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_GENERAL_SWEEP);
      if (Verifyf(pTask, "TaskGeneralSweep Isn't running, so cannot be updated!")) {
        const CEntity* pEntity = CTheScripts::GetEntityToQueryFromGUID<CEntity>(TargetEntityIndex);

        if (Verifyf(pEntity, "Tracking Entity doesn't exist!")) {
          static_cast<CTaskGeneralSweep*>(pTask)->SetTrackEntity(pEntity);
        }
      }
    }
  }

  // Start the sweep task and point at a position
  void CommandTaskSweepAimPosition(int PedIndex, const char* pAnimDictName, const char* pLowAnimName, const char* pMedAnimName, const char* pHiAnimName, int runTime, const scrVector& scrVecCoors, float turnRate, float fBlendInDuration) {
    int clipDictId = fwAnimManager::FindSlot(strStreamingObjectName(pAnimDictName)).Get();
    fwMvClipId lowClipId(pLowAnimName);
    fwMvClipId medClipId(pMedAnimName);
    fwMvClipId highClipId(pHiAnimName);
    const Vector3 AimCoords = Vector3(scrVecCoors);

    aiTask* pTask = rage_new
    CTaskGeneralSweep(clipDictId, lowClipId, medClipId, highClipId, runTime, &AimCoords, turnRate, fBlendInDuration);
    CScriptPeds::GivePedScriptedTask(PedIndex, pTask, SCRIPT_TASK_GENERAL_SWEEP, "TASK_GENERAL_SWEEP");
  }

  // Update the sweep task and point at a position
  void CommandUpdateTaskSweepAimPosition(int PedIndex, const scrVector& scrVecCoors) {
    CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(PedIndex);
    if (pPed) {
      // Check if the task is running
      CTask* pTask = pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_GENERAL_SWEEP);
      if (Verifyf(pTask, "TaskGeneralSweep Isn't running, so cannot be updated!")) {
        const Vector3 AimCoords = Vector3(scrVecCoors);
        static_cast<CTaskGeneralSweep*>(pTask)->SetTrackPosition(&AimCoords);
      }
    }
  }

  void CommandTaskArrestPed(int CopPedIndex, int CrookPedIndex) {
    scriptDisplayf("CommandTaskArrestPed\n");

    CPed* pCopPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(CopPedIndex, 0);
    CPed* pCrookPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(CrookPedIndex, CTheScripts::GUID_ASSERT_FLAGS_INCLUDE_CLONES);
    SCRIPT_ASSERT(pCrookPed, "TASK_ARREST_PED - You must specify the crook ped!");
    if (pCrookPed) {
      if (!pCopPed || !pCopPed->IsAPlayerPed()) {
        CTask* pTask = rage_new
        CTaskArrestPed(pCrookPed);
        CScriptPeds::GivePedScriptedTask(CopPedIndex, pTask, SCRIPT_TASK_ARREST_PED, "TASK_ARREST_PED");
      }
    }
  }

  bool CommandIsPedRunningArrestTask(int PedIndex) {
    const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(PedIndex);
    SCRIPT_ASSERT(pPed, "IS_PED_RUNNING_ARREST_TASK - You must specify the ped!");
    if (pPed) {
      CTaskPlayerOnFoot* pTaskPlayerOnFoot = static_cast< CTaskPlayerOnFoot* >(pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_PLAYER_ON_FOOT));
      if (pTaskPlayerOnFoot) {
        aiTask* pTask = pTaskPlayerOnFoot->GetScriptedTask();
        if (pTask && pTask->GetTaskType() == CTaskTypes::TASK_ARREST_PED2) {
          return true;
        }
      }

      CTaskArrestPed2* pTaskArrestPed2 = static_cast< CTaskArrestPed2* >(pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_ARREST_PED2));
      if (pTaskArrestPed2) {
        return true;
      }
    }

    return false;
  }

  bool CommandIsArrestTypeValid(int CopPedIndex, int CrookPedIndex, int ArrestType) {
    const CPed* pCopPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(CopPedIndex);
    SCRIPT_ASSERT(pCopPed, "IS_ARREST_TYPE_VALID - You must specify the cop ped!");
    const CPed* pCrookPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(CrookPedIndex);
    SCRIPT_ASSERT(pCrookPed, "IS_ARREST_TYPE_VALID - You must specify the crook ped!");
    SCRIPT_ASSERT(ArrestType >= 0 && ArrestType < CArrestHelpers::ARREST_COUNT, "IS_ARREST_TYPE_VALID - You must specify a valid ArrestType!");
    if (pCopPed && pCrookPed && ArrestType >= 0 && ArrestType < CArrestHelpers::ARREST_COUNT) {
      return CArrestHelpers::IsArrestTypeValid(pCopPed, pCrookPed, ArrestType);
    }

    return false;
  }

  bool CommandIsPedBeingArrested(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			CTaskCuffed* pTaskCuffed = static_cast<CTaskCuffed*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CUFFED));
			return pTaskCuffed && pTaskCuffed->IsBeingCuffed();
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  int CommandGetPedArrester(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			CTaskCuffed* pTaskCuffed = static_cast<CTaskCuffed*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CUFFED));
			if (pTaskCuffed && pTaskCuffed->IsBeingCuffed())
			{
				if (CPed* pCustodianPed = pTaskCuffed->GetArrestPed())
				{
					return CTheScripts::GetGUIDFromEntity(*pCustodianPed);
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0;
  }

  bool CommandIsPedCuffed(int iPedIndex) {
    if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex)) {
      // Just check the handcuffed flag rather than checking for the cuffed state,
      // as other higher priority states may have bumped it off temporarily.
      return pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_IsHandCuffed);
    }

    return false;
  }

  int CommandGetPedCustodian(int iPedIndex) {
    if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex)) {
      if (CPed * pCustodianPed = pPed->GetCustodian()) {
        return CTheScripts::GetGUIDFromEntity(*pCustodianPed);
      }
    }

    return 0;
  }

  bool CommandIsPedBeingUncuffed(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			CTaskCuffed* pTaskCuffed = static_cast<CTaskCuffed*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CUFFED));
			return pTaskCuffed && pTaskCuffed->IsBeingUncuffed();
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  int CommandGetPedUncuffer(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			CTaskCuffed* pTaskCuffed = static_cast<CTaskCuffed*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CUFFED));
			if (pTaskCuffed && pTaskCuffed->IsBeingUncuffed())
			{
				if (CPed* pCustodianPed = pTaskCuffed->GetArrestPed())
				{
					return CTheScripts::GetGUIDFromEntity(*pCustodianPed);
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0;
  }

  bool CommandIsPedArresting(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				return pTaskArrest->IsArresting();
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  int CommandGetPedArrestingTarget(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				if (pTaskArrest->IsArresting())
				{
					if (CPed* pTargetPed = pTaskArrest->GetTargetPed())
					{
						return CTheScripts::GetGUIDFromEntity(*pTargetPed);
					}
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0;
  }

  bool CommandIsPedTakingCustody(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				return pTaskArrest->IsTakingCustody();
			}

		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  int CommandGetPedTakingCustodyTarget(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				if (pTaskArrest->IsTakingCustody())
				{
					if (CPed* pTargetPed = pTaskArrest->GetTargetPed())
					{
						return CTheScripts::GetGUIDFromEntity(*pTargetPed);
					}
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0;
  }

  bool CommandIsPedBeingTakenIntoCustody(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if(!pPed->GetPedConfigFlag(CPED_CONFIG_FLAG_IsInCustody))
			{
				CTaskInCustody* pTaskInCustody = static_cast<CTaskInCustody*>(pPed->GetPedIntelligence()->FindTaskByType(CTaskTypes::TASK_IN_CUSTODY));
				if (pTaskInCustody)
				{
					//! Ask in custody task.
					return pTaskInCustody->IsBeingTakenIntoCustody();
				}
				else if(pPed->GetCustodian())
				{
					//! Having a custodian indicates that someone is trying to put you into custody.
					return true;
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  bool CommandIsPedUncuffing(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				return pTaskArrest->IsUnCuffing();
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return false;
  }

  int CommandGetPedUncuffingTarget(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			if (CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST)))
			{
				if(pTaskArrest->IsUnCuffing())
				{
					if (CPed* pTargetPed = pTaskArrest->GetTargetPed())
					{
						return CTheScripts::GetGUIDFromEntity(*pTargetPed);
					}
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0;
  }

  void CommandCuffPed(int iPedIndex) {
    if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex)) {
      if (SCRIPT_VERIFY(!pPed->IsNetworkClone(), "CUFF_PED - Can't cuff cloned ped!")) {
        // Force the ped to be handcuffed immediately
        pPed->SetPedConfigFlag(CPED_CONFIG_FLAG_IsHandCuffed, true);
      }
    }
  }

  void CommandUncuffPed(int iPedIndex) {
    if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex)) {
      if (SCRIPT_VERIFY(!pPed->IsNetworkClone(), "UNCUFF_PED - Can't remove clone ped from cuffs!")) {
        // Just unset the handcuffed flag, this will kick the ped out of the cuffed task
        pPed->SetPedConfigFlag(CPED_CONFIG_FLAG_IsHandCuffed, false);
      }
    }
  }

  void CommandTaskTakeCustody(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iCustodianIndex), int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (CPed* pCustodianPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iCustodianIndex))
		{
			CPed* pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex, 0);
			if(pPed)
			{
				if(SCRIPT_VERIFY(!pPed->IsNetworkClone(), "TASK_TAKE_CUSTODY - Must set on ped owner's, not clone!"))
				{
					pPed->SetInCustody(true, pCustodianPed);
					CTaskInCustody* pTaskInCustody = rage_new CTaskInCustody(pCustodianPed, true);
					CScriptPeds::GivePedScriptedTask(iPedIndex, pTaskInCustody, SCRIPT_TASK_IN_CUSTODY, "TASK_IN_CUSTODY");
				}
			}
		}
    #endif //ENABLE_TASKS_ARREST_CUFFED
  }

  void CommandRemovePedFromCustody(int iPedIndex) {
    if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex)) {
      if (SCRIPT_VERIFY(!pPed->IsNetworkClone(), "REMOVE_PED_FROM_CUSTODY - Can't remove clone ped from custody!")) {
        pPed->SetInCustody(false, NULL);
      }
    }
  }

  float CommandGetPedArrestPhase(int ENABLE_TASKS_ARREST_CUFFED_ONLY(iPedIndex)) {
    #if ENABLE_TASKS_ARREST_CUFFED
                                                                                                                            if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex))
		{
			//! Arrest Task.
			CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST));
			if (pTaskArrest)
			{
				return pTaskArrest->GetPhase();
			}
			//! Cuffed Task
			CTaskCuffed* pTaskCuffed = static_cast<CTaskCuffed*>(pPed->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_CUFFED));
			if (pTaskCuffed)
			{
				return pTaskCuffed->GetPhase();
			}
			//! In Custody Task.
			if(pPed->GetCustodian())
			{
				CTaskArrest* pTaskArrest = static_cast<CTaskArrest*>(pPed->GetCustodian()->GetPedIntelligence()->FindTaskActiveByType(CTaskTypes::TASK_ARREST));
				if(pTaskArrest && pTaskArrest->IsTakingCustody())
				{
					return pTaskArrest->GetPhase();
				}
			}
		}
    #endif // ENABLE_TASKS_ARREST_CUFFED
    return 0.0f;
  }

  int CommandGetPedPotentialArrestTarget(int iPedIndex) {
    if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex)) {
      if (CPed * pTargetPed = pPed->GetArrestTarget()) {
        return CTheScripts::GetGUIDFromEntity(*pTargetPed);
      }
    }

    return 0;
  }

  int CommandGetPedPotentialUncuffTarget(int iPedIndex) {
    if (const CPed* pPed = CTheScripts::GetEntityToQueryFromGUID<CPed>(iPedIndex)) {
      if (CPed * pTargetPed = pPed->GetUncuffTarget()) {
        return CTheScripts::GetGUIDFromEntity(*pTargetPed);
      }
    }

    return 0;
  }

  void CommandSetPedCustodyOverrideFollowDistance(int iPedIndex, float fFollowDistance) {
    if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex)) {
      pPed->SetCustodyFollowDistanceOverride(fFollowDistance);
    }
  }

  void CommandWarpIntoLeadersVehicle(int iPedIndex) {
    if (CPed * pPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedIndex)) {
      pPed->SetPedConfigFlag(CPED_CONFIG_FLAG_WarpIntoLeadersVehicle, true);
    }
  }

  void CommandTaskVehicleGotoNavmesh(int iPedID, int iVehicleID, const scrVector& scrVecCoors,
                                     float fCruiseSpeed, int iDrivingFlags, float fTargetReached) {
    CVehicle* pVehicle = NULL;

    if (SCRIPT_VERIFY(NULL_IN_SCRIPTING_LANGUAGE != iVehicleID, "TASK_VEHICLE_GOTO_NAVMESH - You must specify a vehicle!")) {
      pVehicle = CTheScripts::GetEntityToModifyFromGUID<CVehicle>(iVehicleID);
    }

    if (!pVehicle) {
      return;
    }

    sVehicleMissionParams params;
    params.m_iDrivingFlags = iDrivingFlags;
    if (fCruiseSpeed > 0.0f) {
      params.m_fCruiseSpeed = fCruiseSpeed;
    } else {
      params.m_fCruiseSpeed = 64.0f;
    }
    Vector3 vTargetPos(scrVecCoors);
    params.SetTargetPosition(vTargetPos);
    params.m_fTargetArriveDist = fTargetReached;

    ASSERT_ONLY(params.IsTargetValid());

    CTaskVehicleGoToNavmesh* pNavmeshTask = rage_new
    CTaskVehicleGoToNavmesh(params);

    CTask* pTask = rage_new
    CTaskControlVehicle(pVehicle, pNavmeshTask);
    CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_VEHICLE_GOTO_NAVMESH, "TASK_VEHICLE_GOTO_NAVMESH");
  }

  void CommandGivePedAgitatedTaskConfront(int iPedID, int iAgitatorPedID) {
    if (SCRIPT_VERIFYF((iPedID != NULL_IN_SCRIPTING_LANGUAGE && iAgitatorPedID != NULL_IN_SCRIPTING_LANGUAGE), "TASK_AGITATED_ACTION_CONFRONT_RESPONSE - iPedID (%i) or iAgitatorPedID(%i) is NULL", iPedID, iAgitatorPedID)) {
      CPed* pTargetPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iPedID);
      if (SCRIPT_VERIFYF((pTargetPed && !pTargetPed->IsAPlayerPed()), "TASK_AGITATED_ACTION_CONFRONT_RESPONSE is trying to give agitated task to a player ped or could not find ped with this ID - %i!", iPedID)) {
        CPed* pAgitatorPed = CTheScripts::GetEntityToModifyFromGUID<CPed>(iAgitatorPedID);
        if (SCRIPT_VERIFYF(pAgitatorPed, "TASK_AGITATED_ACTION_CONFRONT_RESPONSE could not find agitator ped with this ID - %i", iAgitatorPedID)) {
          CTaskConfront* pConfrontTask = rage_new
          CTaskConfront(pAgitatorPed);
          CTask* pTask = rage_new
          CTaskAgitatedAction(pConfrontTask);

          pTargetPed->SetPedConfigFlag(CPED_CONFIG_FLAG_CanBeAgitated, true);
          pTargetPed->SetPedConfigFlag(CPED_CONFIG_FLAG_IsAgitated, true);

          CTaskAgitated* pAgitatedTask = rage_new
          CTaskAgitated(pAgitatorPed);
          pAgitatedTask->SetStartedFromScript(true);

          pTargetPed->GetPedIntelligence()->AddTaskSecondary(pAgitatedTask, PED_TASK_SECONDARY_PARTIAL_ANIM);

          CScriptPeds::GivePedScriptedTask(iPedID, pTask, SCRIPT_TASK_AGITATED_ACTION, "TASK_AGITATED_ACTION");
        }
      }
    }
  }

  /////////////////////////////////////////////////


  void SetupScriptCommands() {
    SCR_REGISTER_SECURE(TASK_PAUSE, 0x19c58ba8a663b64b, CommandTaskPause);
    SCR_REGISTER_SECURE(TASK_STAND_STILL, 0x83f5bcfffba26699, CommandTaskStandStill);
    SCR_REGISTER_SECURE(TASK_JUMP, 0x012983f49702416c, CommandTaskJump);
    SCR_REGISTER_SECURE(TASK_COWER, 0x402a537158a551bd, CommandTaskCower);
    SCR_REGISTER_SECURE(TASK_HANDS_UP, 0x3598f95a00026dd8, CommandTaskHandsUp);
    SCR_REGISTER_SECURE(UPDATE_TASK_HANDS_UP_DURATION, 0x46a203feec0ecef4, CommandUpdateTaskHandsUpDuration);
    SCR_REGISTER_UNUSED(TASK_DUCK, 0x54b5addaf8a132f7, CommandTaskDuck);
    SCR_REGISTER_SECURE(TASK_OPEN_VEHICLE_DOOR, 0xf9d446af83e7a624, CommandTaskOpenVehicleDoor);
    SCR_REGISTER_SECURE(TASK_ENTER_VEHICLE, 0xb486640392ec50bb, CommandTaskEnterVehicle);
    SCR_REGISTER_SECURE(TASK_LEAVE_VEHICLE, 0x02f1caac7cb77e47, CommandTaskLeaveVehicle);
    SCR_REGISTER_UNUSED(TASK_MOUNT_ANIMAL, 0x3f0ad0f6b2f34df6, CommandTaskMountAnimal);
    SCR_REGISTER_UNUSED(TASK_DISMOUNT_ANIMAL, 0xfe2e56c600a68fce, CommandTaskDismountAnimal);
    SCR_REGISTER_SECURE(TASK_GET_OFF_BOAT, 0x9aef335a2f73e00a, CommandTaskGetOffBoat);
    SCR_REGISTER_SECURE(TASK_SKY_DIVE, 0x622c61505349f582, CommandTaskSkyDive);
    SCR_REGISTER_SECURE(TASK_PARACHUTE, 0xb7121915a3797c65, CommandTaskParachute);
    SCR_REGISTER_SECURE(TASK_PARACHUTE_TO_TARGET, 0xf323d744a727a4a2, CommandTaskParachuteToTarget);
    SCR_REGISTER_SECURE(SET_PARACHUTE_TASK_TARGET, 0x878fd7e3c8110527, CommandSetParachuteTaskTarget);
    SCR_REGISTER_SECURE(SET_PARACHUTE_TASK_THRUST, 0x3ca038370f6e3223, CommandSetParachuteTaskThrust);
    SCR_REGISTER_UNUSED(SET_PARACHUTE_TASK_TARGET_ENTITY, 0x63b1ba4dbcd822ac, CommandSetParachuteTaskTargetEntity);
    SCR_REGISTER_UNUSED(TASK_JETPACK, 0x23c36cbde291e36c, CommandTaskJetpack);
    SCR_REGISTER_UNUSED(TASK_JETPACK_GO_TO_COORD, 0x586dae563b9e50c6, CommandTaskJetpackGoToCoord);
    SCR_REGISTER_UNUSED(TASK_JETPACK_GO_TO_ENTITY, 0x67794f749799bd7d, CommandTaskJetpackGoToEntity);
    SCR_REGISTER_UNUSED(TASK_JETPACK_SHOOT_AT_COORD, 0x6ebd50cacaab197e, CommandTaskJetpackShootAtCoord);
    SCR_REGISTER_UNUSED(TASK_JETPACK_SHOOT_AT_ENTITY, 0xa767f2d717d074f9, CommandTaskJetpackShootAtEntity);
    SCR_REGISTER_UNUSED(TASK_JETPACK_STOP_SHOOTING, 0xafd3096512ccc6cd, CommandTaskJetpackStopShooting);
    SCR_REGISTER_SECURE(TASK_RAPPEL_FROM_HELI, 0x01ddcf3e0afad31b, CommandTaskRappelFromHeli);
    SCR_REGISTER_UNUSED(TASK_DRAG_PED_TO_COORD, 0xb269aa2c4e7f407c, CommandTaskDragPedToCoord);

    SCR_REGISTER_SECURE(TASK_VEHICLE_DRIVE_TO_COORD, 0x3b8f94419979a0b1, CommandTaskVehicleDriveToCoord);
    SCR_REGISTER_SECURE(TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE, 0x70d024128ce0c114, CommandTaskVehicleDriveToCoordLongRange);
    SCR_REGISTER_SECURE(TASK_VEHICLE_DRIVE_WANDER, 0xad6cdbcda713560a, CommandTaskVehicleDriveWander);
    SCR_REGISTER_SECURE(TASK_FOLLOW_TO_OFFSET_OF_ENTITY, 0x6fce069e4940878c, CommandTaskFollowToOffsetOfEntity);
    SCR_REGISTER_UNUSED(TASK_FOLLOW_TO_OFFSET_OF_PICKUP, 0x6f91735acae34873, CommandTaskFollowToOffsetOfPickup);
    SCR_REGISTER_SECURE(TASK_GO_STRAIGHT_TO_COORD, 0x13c3030981ea7c3b, CommandTaskGoStraightToCoord);
    SCR_REGISTER_SECURE(TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY, 0xc98e8d7f0d9d4795, CommandTaskGoStraightToCoordRelativeToEntity);
    SCR_REGISTER_SECURE(TASK_ACHIEVE_HEADING, 0x30e00b59448ab97e, CommandTaskAchieveHeading);
    SCR_REGISTER_SECURE(TASK_FLUSH_ROUTE, 0x83a64e1fe4669225, CommandFlushRoute);
    SCR_REGISTER_SECURE(TASK_EXTEND_ROUTE, 0x9d38259cb54342be, CommandExtendRoute);
    SCR_REGISTER_SECURE(TASK_FOLLOW_POINT_ROUTE, 0x790dea4e63d61baa, CommandTaskFollowPointRoute);
    SCR_REGISTER_SECURE(TASK_GO_TO_ENTITY, 0xc149e50fbb27dd70, CommandTaskGoToEntity);
    SCR_REGISTER_SECURE(TASK_SMART_FLEE_COORD, 0x84918839a90c954c, CommandTaskSmartFleeCoord);
    SCR_REGISTER_SECURE(TASK_SMART_FLEE_PED, 0x7734082b0edb0be0, CommandTaskSmartFleePed);
    SCR_REGISTER_UNUSED(TASK_REACT_AND_FLEE_COORD, 0x596713d55325f5c9, CommandTaskReactAndFleeCoord);
    SCR_REGISTER_SECURE(TASK_REACT_AND_FLEE_PED, 0x7a9da0518e81ec15, CommandTaskReactAndFleePed);
    SCR_REGISTER_UNUSED(TASK_SHOCKING_EVENT_BACK_AWAY, 0xa19ac1e1c0d01b42, CommandTaskShockingEventBackAway);
    SCR_REGISTER_UNUSED(TASK_SHOCKING_EVENT_HURRY_AWAY, 0x4e742aa861140b96, CommandTaskShockingEventHurryAway);
    SCR_REGISTER_SECURE(TASK_SHOCKING_EVENT_REACT, 0xb5bc9e2b43334cef, CommandTaskShockingEventReact);
    SCR_REGISTER_SECURE(TASK_WANDER_IN_AREA, 0x5315f7ac22dc95f1, CommandTaskWanderInArea);
    SCR_REGISTER_SECURE(TASK_WANDER_STANDARD, 0xd86a0bc9cc0a625a, CommandTaskWanderStandard);
    SCR_REGISTER_SECURE(TASK_WANDER_SPECIFIC, 0x26f1da9738c260f2, CommandTaskWanderSpecific);
    SCR_REGISTER_SECURE(TASK_VEHICLE_PARK, 0x5b7af57d366861f2, CommandTaskVehiclePark);

    SCR_REGISTER_SECURE(TASK_STEALTH_KILL, 0xe2c1d6fa4e5ea991, CommandTaskStealthKill);
    SCR_REGISTER_SECURE(TASK_PLANT_BOMB, 0xd9ab2b08ce49a043, CommandTaskPlantBomb);

    SCR_REGISTER_UNUSED(TASK_SHARK_CIRCLE_COORD, 0xfff1232e7a485388, CommandTaskSharkCircleCoord);
    SCR_REGISTER_UNUSED(TASK_SHARK_CIRCLE_PED, 0xfd4e7488138e4f25, CommandTaskSharkCirclePed);

    SCR_REGISTER_SECURE(TASK_FOLLOW_NAV_MESH_TO_COORD, 0x7d1424753688ee7a, CommandTaskFollowNavMeshToCoord);
    scrThread::RegisterCommand(SCRHASH("TASK_FOLLOW_NAV_MESH_TO_COORD_ADVANCED", 0x72f317bc03266125), CommandTaskFollowNavMeshToCoordAdvanced_ScrStruct
    SCRIPT_DEBUGGING_ONLY(, "TASK_FOLLOW_NAV_MESH_TO_COORD_ADVANCED", __FILE__, scrSignature()));

    SCR_REGISTER_SECURE(SET_PED_PATH_CAN_USE_CLIMBOVERS, 0xcacf57ad414acc75, CommandSetPedPathCanUseClimbOvers);
    SCR_REGISTER_SECURE(SET_PED_PATH_CAN_USE_LADDERS, 0x32bb80e6e576379a, CommandSetPedPathCanUseLadders);
    SCR_REGISTER_SECURE(SET_PED_PATH_CAN_DROP_FROM_HEIGHT, 0x91d9c55e11392690, CommandSetPedPathCanDropFromHeight);
    SCR_REGISTER_SECURE(SET_PED_PATH_CLIMB_COST_MODIFIER, 0xd0b0e285f00a8b61, CommandSetPedPathClimbCostModifier);
    SCR_REGISTER_SECURE(SET_PED_PATH_MAY_ENTER_WATER, 0xbe74e997fb714956, CommandSetPedToMayEnterWater);
    SCR_REGISTER_SECURE(SET_PED_PATH_PREFER_TO_AVOID_WATER, 0x8bb90eda0a7b150e, CommandSetPedPreferToAvoidWater);
    SCR_REGISTER_SECURE(SET_PED_PATH_AVOID_FIRE, 0x9ef69bf12b6fbcb3, CommandSetPedPathAvoidFire);

    SCR_REGISTER_SECURE(SET_GLOBAL_MIN_BIRD_FLIGHT_HEIGHT, 0xb133bc406357c5a5, CommandSetGlobalMinBirdFlightHeight);

    SCR_REGISTER_SECURE(GET_NAVMESH_ROUTE_DISTANCE_REMAINING, 0x9fa616df1ef7172a, CommandGetNavMeshRouteDistanceRemaining);
    SCR_REGISTER_SECURE(GET_NAVMESH_ROUTE_RESULT, 0xb02b2cd950b6ba04, CommandGetNavMeshRouteResult);
    SCR_REGISTER_SECURE(IS_CONTROLLED_VEHICLE_UNABLE_TO_GET_TO_ROAD, 0xa8a55d2ba7497f6f, CommandIsControlledVehicleUnableToGetToRoad);

    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_ANY_MEANS, 0x9603c0ec536c6425, CommandTaskGoToCoordAnyMeans);
    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS, 0x110dedd855267ad4, CommandTaskGoToCoordAnyMeansExtraParams);
    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED, 0xfaaa6a6a67097054, CommandTaskGoToCoordAnyMeansExtraParamsWithCruiseSpeed);

    //////////////////////////////////////////////////////////////////////////
    //Animation task commands
    //////////////////////////////////////////////////////////////////////////

    SCR_REGISTER_SECURE(TASK_PLAY_ANIM, 0x49d97b076e3590ac, CommandTaskPlayAnim);
    SCR_REGISTER_SECURE(TASK_PLAY_ANIM_ADVANCED, 0x246c656f8e525dc7, CommandTaskPlayAnimAdvanced);
    SCR_REGISTER_SECURE(STOP_ANIM_TASK, 0x227b2dd85a708e68, CommandStopAnimTask);

    SCR_REGISTER_SECURE(TASK_SCRIPTED_ANIMATION, 0x86577f7f045f84c7, CommandTaskScriptedAnimation);
    SCR_REGISTER_SECURE(PLAY_ENTITY_SCRIPTED_ANIM, 0xc51d165944c64ca6, CommandPlayEntityScriptedAnimation);
    SCR_REGISTER_UNUSED(START_ANIM_PLAYBACK, 0x8fc2aa42fd9fa172, CommandStartAnimPlayback);
    SCR_REGISTER_SECURE(STOP_ANIM_PLAYBACK, 0xe3258b995b832e6a, CommandStopAnimPlayback);
    SCR_REGISTER_UNUSED(SET_ANIM_CLIP, 0xf962040546b0d2d7, CommandTaskClipSetClip);
    SCR_REGISTER_SECURE(SET_ANIM_WEIGHT, 0xecd4da16b454528f, CommandTaskClipSetBlendWeight);
    SCR_REGISTER_UNUSED(SET_ANIM_FILTER, 0x979aba5e5d177c74, CommandTaskClipSetFilter);
    SCR_REGISTER_SECURE(SET_ANIM_PHASE, 0x60f9850ce23ed209, CommandTaskClipSetPhase);
    SCR_REGISTER_SECURE(SET_ANIM_RATE, 0x1cbf84e41b1bfe92, CommandTaskClipSetRate);
    SCR_REGISTER_SECURE(SET_ANIM_LOOPED, 0x8bf7b8753e6fadd0, CommandTaskClipSetLooped);

    SCR_REGISTER_SECURE(TASK_PLAY_PHONE_GESTURE_ANIMATION, 0xb8ea44dc2c77645c, CommandTaskPlayPhoneGestureAnimation);
    SCR_REGISTER_SECURE(TASK_STOP_PHONE_GESTURE_ANIMATION, 0xfcd42d914e921580, CommandTaskStopPhoneGestureAnimation);
    SCR_REGISTER_SECURE(IS_PLAYING_PHONE_GESTURE_ANIM, 0x7ce0cfe0533867b0, CommandIsPlayingPhoneGestureAnim);
    SCR_REGISTER_SECURE(GET_PHONE_GESTURE_ANIM_CURRENT_TIME, 0x198866ca14785166, CommandGetPhoneGestureAnimCurrentTime);
    SCR_REGISTER_SECURE(GET_PHONE_GESTURE_ANIM_TOTAL_TIME, 0xc20701b83e0998c5, CommandGetPhoneGestureAnimTotalTime);

    SCR_REGISTER_SECURE(TASK_VEHICLE_PLAY_ANIM, 0x8da8932b29e225ed, CommandVehicleTaskPlayAnim);
    SCR_REGISTER_SECURE(TASK_LOOK_AT_COORD, 0xe1e1af00ca06a2b7, CommandTaskLookAtCoord);
    SCR_REGISTER_SECURE(TASK_LOOK_AT_ENTITY, 0xf30f15f203736de4, CommandTaskLookAtEntity);
    SCR_REGISTER_SECURE(TASK_CLEAR_LOOK_AT, 0xc89ea639a6f338a6, CommandClearLookAt);

    SCR_REGISTER_SECURE(OPEN_SEQUENCE_TASK, 0xa888f8cc04f25cc8, CommandOpenSequenceTask);
    SCR_REGISTER_SECURE(CLOSE_SEQUENCE_TASK, 0xc963a45b50851768, CommandCloseSequenceTask);
    SCR_REGISTER_SECURE(TASK_PERFORM_SEQUENCE, 0x8acada903fcaa42f, CommandTaskPerformSequence);
    SCR_REGISTER_SECURE(TASK_PERFORM_SEQUENCE_LOCALLY, 0x7bcccccdabd49a93, CommandTaskPerformSequenceLocally);

    SCR_REGISTER_SECURE(CLEAR_SEQUENCE_TASK, 0x7461d7c5ba953bc7, CommandClearSequenceTask);
    SCR_REGISTER_SECURE(SET_SEQUENCE_TO_REPEAT, 0x3f2b528df27c8a9f, CommandSetSequenceToRepeat);
    SCR_REGISTER_UNUSED(SET_SEQUENCE_PREVENT_MIGRATION, 0x6a177d7d156481ae, CommandSetSequencePreventMigration);
    SCR_REGISTER_SECURE(GET_SEQUENCE_PROGRESS, 0x0829092f169950ab, CommandGetSequenceProgress);

    SCR_REGISTER_SECURE(GET_IS_TASK_ACTIVE, 0x7f528e84564c4d10, CommandGetIsTaskActive);

    SCR_REGISTER_SECURE(GET_SCRIPT_TASK_STATUS, 0x174ced88b97c78d9, CommandGetScriptTaskStatus);
    SCR_REGISTER_SECURE(GET_ACTIVE_VEHICLE_MISSION_TYPE, 0xf9eec73ae0b9c439, CommandGetActiveVehicleMissionType);
    SCR_REGISTER_SECURE(TASK_LEAVE_ANY_VEHICLE, 0xab7639d658bbccee, CommandTaskLeaveAnyVehicle);
    SCR_REGISTER_SECURE(TASK_AIM_GUN_SCRIPTED, 0xccf862b807fe3901, CommandTaskAimGunScripted);
    SCR_REGISTER_SECURE(TASK_AIM_GUN_SCRIPTED_WITH_TARGET, 0x30cd3d58df974a16, CommandTaskAimGunScriptedWithTarget);
    SCR_REGISTER_SECURE(UPDATE_TASK_AIM_GUN_SCRIPTED_TARGET, 0x20a2aeaf04674323, CommmandUpdateTaskAimGunScriptedTarget);
    SCR_REGISTER_SECURE(GET_CLIP_SET_FOR_SCRIPTED_GUN_TASK, 0x3943988de42ef74a, CommandGetClipSetForScriptedGunTask);
    SCR_REGISTER_UNUSED(SET_ROPE_TRACK_ENTITY_FOR_SCRIPTED_GUN_TASK, 0x87ac327b490ecccc, CommandSetRopeTrackEntityForGunTask);

    SCR_REGISTER_SECURE(TASK_AIM_GUN_AT_ENTITY, 0x636b3584208a6d73, CommandTaskAimGunAtEntity);
    SCR_REGISTER_SECURE(TASK_TURN_PED_TO_FACE_ENTITY, 0x529d306014d3c29a, CommandTaskTurnPedToFaceEntity);
    SCR_REGISTER_SECURE(TASK_AIM_GUN_AT_COORD, 0x2ea72cdbfacd9e92, CommandTaskAimGunAtCoord);
    SCR_REGISTER_SECURE(TASK_SHOOT_AT_COORD, 0xa99ad9dee4794c48, CommandTaskShootAtCoord);
    SCR_REGISTER_SECURE(TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT, 0x8625186d3a8ddd7a, CommandTaskShuffleToNextVehicleSeat);
    SCR_REGISTER_SECURE(CLEAR_PED_TASKS, 0x03a927199a2dfe46, CommandClearPedTasks);
    SCR_REGISTER_UNUSED(CLEAR_PED_SCRIPT_TASK_IF_RUNNING_THREAT_RESPONSE_NON_TEMP_TASK, 0xf3642376bf6ac7a8, CommandClearPedScriptTaskIfRunningThreatResponseNonTempTask);

    SCR_REGISTER_SECURE(CLEAR_PED_SECONDARY_TASK, 0xca2872f050840231, CommandClearPedSecondaryTask);
    SCR_REGISTER_SECURE(TASK_EVERYONE_LEAVE_VEHICLE, 0x417047b1f659225c, CommandTaskEveryoneLeaveVehicle);
    SCR_REGISTER_SECURE(TASK_GOTO_ENTITY_OFFSET, 0x6624b56c8f9a7bbf, CommandTaskGotoEntityOffset);
    SCR_REGISTER_SECURE(TASK_GOTO_ENTITY_OFFSET_XY, 0xb17a6034b9f1bcc2, CommandTaskGotoEntityOffsetXY);
    SCR_REGISTER_UNUSED(TASK_INVESTIGATE_COORDS, 0xb834945c258dac7b, CommandTaskInvestigateCoords);

    SCR_REGISTER_SECURE(TASK_TURN_PED_TO_FACE_COORD, 0x1a96b8b416bc07d0, CommandTaskTurnPedToFaceCoord);
    SCR_REGISTER_UNUSED(TASK_DRIVE_POINT_ROUTE, 0x8c4f2dd2f4b46da9, CommandTaskDrivePointRoute);
    SCR_REGISTER_SECURE(TASK_VEHICLE_TEMP_ACTION, 0xcd41d6f721cb9c3a, CommandTaskVehicleTempAction);
    SCR_REGISTER_SECURE(TASK_VEHICLE_MISSION, 0xfe5a02cf2178b6a8, CommandTaskVehicleMission);
    SCR_REGISTER_SECURE(TASK_VEHICLE_MISSION_PED_TARGET, 0xb69d13e19147910f, CommandTaskVehicleMissionPedTarget);
    SCR_REGISTER_SECURE(TASK_VEHICLE_MISSION_COORS_TARGET, 0xfdb121077308e944, CommandTaskVehicleMissionCoorsTarget);
    SCR_REGISTER_UNUSED(IS_PED_INVESTIGATING_WHISTLING_EVENT, 0x7fd44d022e2a2e07, CommandIsPedInvestigatingWhistlingEvent);
    SCR_REGISTER_UNUSED(SET_INVESTIGATION_POSITION, 0x00f6e31f2f0528cc, CommandSetInvestigationPosition);
    SCR_REGISTER_SECURE(TASK_VEHICLE_ESCORT, 0xdbee353c7ca6e88e, CommandTaskVehicleEscort);
    SCR_REGISTER_SECURE(TASK_VEHICLE_FOLLOW, 0xbb97101b3b2dbef5, CommandTaskVehicleFollow);
    SCR_REGISTER_SECURE(TASK_VEHICLE_CHASE, 0x9286a110a3fd81f1, CommandTaskVehicleChase);
    SCR_REGISTER_SECURE(TASK_VEHICLE_HELI_PROTECT, 0x69ac7dd318a3e1ba, CommandTaskHeliProtect);

    SCR_REGISTER_SECURE(SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG, 0xc8d0b0d2cfd76bb5, CommandSetTaskVehicleChaseBehaviorFlag);
    SCR_REGISTER_SECURE(SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE, 0xdfad0df477fd9eb3, CommandSetTaskVehicleChaseIdealPursuitDistance);
    SCR_REGISTER_SECURE(TASK_HELI_CHASE, 0x42935fa7528ffce8, CommandTaskHeliChase);
    SCR_REGISTER_UNUSED(SET_TASK_HELI_CHASE_TARGET_OFFSET, 0x4caff843df59d524, CommandSetTaskHeliChaseTargetOffset);
    SCR_REGISTER_UNUSED(SET_TASK_HELI_CHASE_TARGET_OFFSET_WORLD_SPACE, 0x8d6ba126f7ac98e4, CommandSetTaskHeliChaseTargetOffsetWorldSpace);
    SCR_REGISTER_UNUSED(SET_TASK_HELI_CHASE_ORIENTATION, 0xe97d050a690da21c, CommandSetTaskHeliChaseOrientation);

    SCR_REGISTER_SECURE(TASK_PLANE_CHASE, 0xe8f8c382b0eb7478, CommandTaskPlaneChase);
    SCR_REGISTER_SECURE(TASK_PLANE_LAND, 0x214f418bf9481244, CommandTaskPlaneLand);

    SCR_REGISTER_SECURE(CLEAR_DEFAULT_PRIMARY_TASK, 0xeb6fbc20375e72c2, CommandClearDefaultPrimaryTask);
    SCR_REGISTER_SECURE(CLEAR_PRIMARY_VEHICLE_TASK, 0xe4a094de05c0210c, CommandClearPrimaryVehicleTask);
    SCR_REGISTER_SECURE(CLEAR_VEHICLE_CRASH_TASK, 0x273bc6472375634c, CommandClearVehicleCrashTask);
    SCR_REGISTER_SECURE(TASK_PLANE_GOTO_PRECISE_VTOL, 0xce661a3c8cf58b44, CommandTaskPlaneGotoPreciseVtol);
    SCR_REGISTER_SECURE(TASK_SUBMARINE_GOTO_AND_STOP, 0x835cbdb247b09c67, CommandTaskSubmarineGotoAndStop);
    SCR_REGISTER_SECURE(TASK_HELI_MISSION, 0x545c59578966d5b8, CommandTaskHeliMission);
    SCR_REGISTER_SECURE(TASK_HELI_ESCORT_HELI, 0x53a1d75e294fdb30, CommandTaskHeliEscort);
    SCR_REGISTER_SECURE(TASK_PLANE_MISSION, 0xcdaacb23d43f080d, CommandTaskPlaneMission);
    SCR_REGISTER_SECURE(TASK_PLANE_TAXI, 0x908b77e6d8aa83d8, CommandTaskPlaneTaxi);

    SCR_REGISTER_SECURE(TASK_BOAT_MISSION, 0xc5a824d6ee9c9cd3, CommandTaskBoatMission);

    SCR_REGISTER_UNUSED(TASK_WEAPON_ROLL, 0x123b2716, CommandTaskWeaponRoll);

    SCR_REGISTER_SECURE(TASK_DRIVE_BY, 0x0e6c3a4efa9d9d35, CommandTaskDriveBy);
    SCR_REGISTER_SECURE(SET_DRIVEBY_TASK_TARGET, 0x8217f7dfe30383b0, CommandSetDriveByTarget);
    SCR_REGISTER_SECURE(CLEAR_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK, 0xd639b91d6baf25c6, CommandClearDrivebyTaskUnderneathDrivingTask);
    SCR_REGISTER_SECURE(IS_DRIVEBY_TASK_UNDERNEATH_DRIVING_TASK, 0x99d3be8c39ba34ff, CommandIsDrivebyTaskUnderneathDrivingTask);

    SCR_REGISTER_SECURE(CONTROL_MOUNTED_WEAPON, 0xb604014eee3ca5f8, CommandControlMountedWeapon);
    SCR_REGISTER_SECURE(SET_MOUNTED_WEAPON_TARGET, 0x748e7aee8deb2a3a, CommandSetMountedWeaponTarget);
    SCR_REGISTER_UNUSED(CLEAR_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK, 0xf4dedafc0c550318, CommandClearMountedWeaponTaskUnderneathDrivingTask);
    SCR_REGISTER_SECURE(IS_MOUNTED_WEAPON_TASK_UNDERNEATH_DRIVING_TASK, 0xcb8e14dc7871f52c, CommandIsMountedWeaponTaskUnderneathDrivingTask);

    SCR_REGISTER_SECURE(TASK_USE_MOBILE_PHONE, 0x57caa5fbf134d4ae, CommandTaskUseMobilePhone);
    SCR_REGISTER_SECURE(TASK_USE_MOBILE_PHONE_TIMED, 0x9bd5d84c8c3f0b2c, CommandTaskUseMobilePhoneTimed);
    SCR_REGISTER_UNUSED(TASK_USE_WALKIE_TALKIE, 0x46f7b490eb75e92c, CommandTaskUseWalkieTalkie);
    SCR_REGISTER_SECURE(TASK_CHAT_TO_PED, 0x2966111306fa6611, CommandTaskChatToPed);
    SCR_REGISTER_UNUSED(IS_CHATTING_PED_IN_POSITION, 0xd701a91d13919148, CommandIsChattingPedInPosition);
    SCR_REGISTER_UNUSED(IS_CHATTING_PED_PLAYING_ANIM, 0xae8fabcda3959d22, CommandIsChattingPedPlayingAnim);
    SCR_REGISTER_UNUSED(MAKE_CHATTING_PED_PLAY_ANIM, 0x3f4bfc1622d674be, CommandMakeChattingPedPlayAnim);
    SCR_REGISTER_UNUSED(MAKE_CHATTING_PED_LEAVE, 0x9dfbb232fdaa2244, CommandMakeChattingPedLeave);

    SCR_REGISTER_UNUSED(ADD_FOLLOW_NAVMESH_TO_PHONE_TASK, 0xe12df85ba18a27f8, CommandAddFollowNavmeshToPhoneTask);

    SCR_REGISTER_UNUSED(GET_MOBILE_PHONE_TASK_SUB_TASK, 0xeb6576b40ae232f5, CommandGetMobilePhoneTaskSubTask);
    SCR_REGISTER_SECURE(TASK_WARP_PED_INTO_VEHICLE, 0x73a521eac1ef5c1b, CommandTaskWarpPedIntoVehicle);

    SCR_REGISTER_SECURE(TASK_SHOOT_AT_ENTITY, 0x913e191f1f726271, CommandTaskShootAtEntity);
    SCR_REGISTER_SECURE(TASK_CLIMB, 0x893ddd76cc7c67b1, CommandTaskClimb);
    SCR_REGISTER_SECURE(TASK_CLIMB_LADDER, 0x12425bd281be5f3c, CommandTaskClimbLadder);
    SCR_REGISTER_UNUSED(TASK_RAPPEL_DOWN_WALL, 0x42ee24d502e263b3, CommandTaskRappelDownWall);
    SCR_REGISTER_SECURE(TASK_RAPPEL_DOWN_WALL_USING_CLIPSET_OVERRIDE, 0x80f9b0598b0c9c85, CommandTaskRappelDownWallUsingClipsetOverride);
    SCR_REGISTER_SECURE(GET_TASK_RAPPEL_DOWN_WALL_STATE, 0xa0584425edd6ec9d, CommandsGetTaskRappelDownWallState);
    SCR_REGISTER_SECURE(CLEAR_PED_TASKS_IMMEDIATELY, 0x51bb443b279e4104, CommandClearPedTasksImmediately);
    SCR_REGISTER_SECURE(TASK_PERFORM_SEQUENCE_FROM_PROGRESS, 0x23378ce912d5b544, CommandTaskPerformSequenceFromProgress);
    SCR_REGISTER_SECURE(SET_NEXT_DESIRED_MOVE_STATE, 0x8ff5f2b541a0e471, CommandSetNextDesiredMoveState);
    SCR_REGISTER_SECURE(SET_PED_DESIRED_MOVE_BLEND_RATIO, 0x98f7e0916b6b5a0b, CommandSetPedDesiredMoveBlendRatio);
    SCR_REGISTER_SECURE(GET_PED_DESIRED_MOVE_BLEND_RATIO, 0x5e9b3bbf63a5e2f6, CommandGetPedDesiredMoveBlendRatio);

    SCR_REGISTER_SECURE(TASK_GOTO_ENTITY_AIMING, 0x27c6152f3e5f2933, CommandTaskGotoEntityAiming);
    SCR_REGISTER_SECURE(TASK_SET_DECISION_MAKER, 0xd029ba670c8df65b, CommandTaskSetDecisionMaker);
    SCR_REGISTER_SECURE(TASK_SET_SPHERE_DEFENSIVE_AREA, 0x223133feb94059cd, CommandTaskSetSphereDefensiveArea);
    SCR_REGISTER_SECURE(TASK_CLEAR_DEFENSIVE_AREA, 0x4b16afbb0c12b630, CommandTaskClearDefensiveArea);
    SCR_REGISTER_SECURE(TASK_PED_SLIDE_TO_COORD, 0xed2842d9cc50eb6e, CommandTaskPedSlideToCoord);
    SCR_REGISTER_UNUSED(TASK_PED_SLIDE_TO_COORD_AND_PLAY_ANIM, 0x93a741ed3ed7f56f, CommandTaskPedSlideToCoordAndPlayAnim);
    SCR_REGISTER_UNUSED(TASK_PED_SLIDE_TO_COORD_AND_PLAY_ANIM_HDG_RATE, 0x95abc676, CommandTaskPedSlideToCoordAndPlayAnimWithHeadingChangeRate);
    SCR_REGISTER_SECURE(TASK_PED_SLIDE_TO_COORD_HDG_RATE, 0xd6711eee6b632dae, CommandTaskPedSlideToCoordWithHeadingChangeRate);
    SCR_REGISTER_UNUSED(TASK_DRIVE_POINT_ROUTE_ADVANCED, 0x730dad3cf7f02fa7, CommandTaskDrivePointRouteAdvanced);

    // Combat and cover
    SCR_REGISTER_SECURE(ADD_COVER_POINT, 0x41ba9dba367c9c12, CommandAddCoverPoint);
    SCR_REGISTER_SECURE(REMOVE_COVER_POINT, 0x5990dad3727a1b45, CommandRemoveCoverPoint);
    SCR_REGISTER_SECURE(DOES_SCRIPTED_COVER_POINT_EXIST_AT_COORDS, 0x7fd113b09d96c678, CommandDoesScriptedCoverPointExistAtCoords);
    SCR_REGISTER_SECURE(GET_SCRIPTED_COVER_POINT_COORDS, 0x137fb6fc6e40a0a8, CommandGetScriptedCoverPointCoords);
    SCR_REGISTER_SECURE(ADD_SCRIPTED_COVER_AREA, 0x28b7b9bfdaf274aa, CommandAddScriptedCoverArea);
    //SCR_REGISTER_UNUSED(GET_SCRIPTED_COVER_POINT_STATUS, 0x56c9de8d,			CommandGetScriptedCoverPointStatus		);
    SCR_REGISTER_SECURE(TASK_COMBAT_PED, 0xc1a74225341aa9fb, CommandTaskCombat);
    SCR_REGISTER_SECURE(TASK_COMBAT_PED_TIMED, 0x5fc4a511155d6f9a, CommandTaskCombatTimed);
    SCR_REGISTER_SECURE(TASK_SEEK_COVER_FROM_POS, 0x3f58924930345d8d, CommandTaskSeekCoverFromPos);
    SCR_REGISTER_SECURE(TASK_SEEK_COVER_FROM_PED, 0xb67d1d267c666dc4, CommandTaskSeekCoverFromPed);
    SCR_REGISTER_SECURE(TASK_SEEK_COVER_TO_COVER_POINT, 0x419e449559bf60b4, CommandTaskSeekCoverToCoverPoint);
    SCR_REGISTER_SECURE(TASK_SEEK_COVER_TO_COORDS, 0x264cd1b504831626, CommandTaskSeekCoverToCoords);
    SCR_REGISTER_SECURE(TASK_PUT_PED_DIRECTLY_INTO_COVER, 0x809dfa39515d5981, CommandTaskPutPedDirectlyIntoCover);
    SCR_REGISTER_UNUSED(TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET, 0x9f8e54193d416a78, CommandTaskPutPedDirectlyIntoCoverFromTarget);
    SCR_REGISTER_SECURE(TASK_WARP_PED_DIRECTLY_INTO_COVER, 0x6e01e9e8d89f8276, CommandTaskWarpPedDirectlyIntoCover);
    SCR_REGISTER_UNUSED(SET_COVER_TASK_TARGET, 0x778c4e702f2a65e5, CommandSetCoverTarget);
    SCR_REGISTER_SECURE(TASK_EXIT_COVER, 0x412ed6ee0b28c20d, CommandTaskExitCover);
    SCR_REGISTER_SECURE(TASK_PUT_PED_DIRECTLY_INTO_MELEE, 0x957211dc68d2869f, CommandTaskPutPedDirectlyIntoMelee);
    SCR_REGISTER_UNUSED(TASK_ADVANCE_TO_TARGET_IN_LINE, 0xd9c691ad25d0d5eb, CommandTaskAdvanceToTargetInLine);

    SCR_REGISTER_UNUSED(SET_CHARGE_TARGET_ENABLED, 0x9dea9d830bfc9937, CommandSetChargeTargetEnabled);
    SCR_REGISTER_UNUSED(SET_MAX_NUM_ACTIVE_CHARGERS, 0xa2d85c968ea44e3d, CommandSetMaxNumActiveChargers);
    SCR_REGISTER_UNUSED(SET_MIN_TIME_BETWEEN_CHARGES_AT_SAME_TARGET, 0xfbb868ae331debcd, CommandSetMinTimeBetweenChargesAtSameTarget);

    SCR_REGISTER_SECURE(TASK_TOGGLE_DUCK, 0xa5af895dd224fdc0, CommandTaskToggleDuck);

    SCR_REGISTER_SECURE(TASK_GUARD_CURRENT_POSITION, 0x274a180d57a0d9a6, CommandTaskGuardCurrentPosition);
    SCR_REGISTER_SECURE(TASK_GUARD_ASSIGNED_DEFENSIVE_AREA, 0x4182d960e26d40bb, CommandTaskGuardAssignedDefensiveArea);
    SCR_REGISTER_UNUSED(TASK_GUARD_ANGLED_DEFENSIVE_AREA, 0xf035a69189e11b7a, CommandTaskGuardAngledDefensiveArea);
    SCR_REGISTER_SECURE(TASK_GUARD_SPHERE_DEFENSIVE_AREA, 0x061a42ebc9ceb366, CommandTaskGuardSphereDefensiveArea);
    SCR_REGISTER_SECURE(TASK_STAND_GUARD, 0x4b551665e9b2e8f6, CommandTaskStandGuard);

    SCR_REGISTER_SECURE(SET_DRIVE_TASK_CRUISE_SPEED, 0xbb3480bb855cdb33, CommandSetDriveTaskCruiseSpeed);
    SCR_REGISTER_SECURE(SET_DRIVE_TASK_MAX_CRUISE_SPEED, 0x3d2ebac0a745e60c, CommandSetDriveTaskMaxCruiseSpeed);
    SCR_REGISTER_SECURE(SET_DRIVE_TASK_DRIVING_STYLE, 0x27be5555cdf6f983, CommandSetDriveTaskDrivingStyle);
    SCR_REGISTER_UNUSED(SET_PURSUE_TASK_IDEAL_DISTANCE, 0x971919670679e003, CommandSetPursueTaskIdealDistance);

    SCR_REGISTER_SECURE(ADD_COVER_BLOCKING_AREA, 0x5af3192f3c3d45ec, CommandAddCoverBlockingArea);
    SCR_REGISTER_SECURE(REMOVE_ALL_COVER_BLOCKING_AREAS, 0x71fcd88fc7f98621, CommandFlushCoverBlockingAreas);
    SCR_REGISTER_SECURE(REMOVE_COVER_BLOCKING_AREAS_AT_POSITION, 0xac7ab29fe3c36266, CommandRemoveCoverBlockingAreasAtPosition);
    SCR_REGISTER_SECURE(REMOVE_SPECIFIC_COVER_BLOCKING_AREAS, 0xaa443863046f380a, CommandRemoveSpecificCoverBlockingAreas);

    SCR_REGISTER_SECURE(TASK_START_SCENARIO_IN_PLACE, 0xc9fefb406c44f60b, CommandTaskStartScenarioInPlace);
    SCR_REGISTER_SECURE(TASK_START_SCENARIO_AT_POSITION, 0xdb4f67aafbf32aa5, CommandTaskStartScenarioAtPosition);
    SCR_REGISTER_UNUSED(TASK_START_VEHICLE_SCENARIO, 0xbfb160b082e439fa, CommandTaskStartVehicleScenario);
    SCR_REGISTER_SECURE(TASK_USE_NEAREST_SCENARIO_TO_COORD, 0x1d698ecdff42ac58, CommandTaskUseNearestScenarioToPos);
    SCR_REGISTER_SECURE(TASK_USE_NEAREST_SCENARIO_TO_COORD_WARP, 0x8ceb5d8b56025aa2, CommandTaskUseNearestScenarioToPosWarp);
    SCR_REGISTER_UNUSED(TASK_USE_NEAREST_TRAIN_SCENARIO_TO_COORD_WARP, 0x97deba5039a7f6c7, CommandTaskUseNearestTrainScenarioToPosWarp);
    SCR_REGISTER_SECURE(TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD, 0xaf0492886bc6c105, CommandTaskUseNearestScenarioChainToPos);
    SCR_REGISTER_SECURE(TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD_WARP, 0x6d44636281842060, CommandTaskUseNearestScenarioChainToPosWarp);
    SCR_REGISTER_SECURE(DOES_SCENARIO_EXIST_IN_AREA, 0x5b138084858689ac, CommandDoesScenarioExistInArea);
    SCR_REGISTER_SECURE(DOES_SCENARIO_OF_TYPE_EXIST_IN_AREA, 0x48d8c2729764de01, CommandDoesScenarioOfTypeExistInArea);
    SCR_REGISTER_SECURE(IS_SCENARIO_OCCUPIED, 0xc85b76c87214bd87, CommandIsScenarioOccupied);
    SCR_REGISTER_SECURE(PED_HAS_USE_SCENARIO_TASK, 0x23bf8abbc9594609, CommandPedHasUseScenarioTask);
    SCR_REGISTER_UNUSED(OVERRIDE_TASKED_SCENARIO_BASE_ANIM, 0xdd7e39f6a9fa3ce0, CommandOverrideTaskedScenarioBaseAnim);
    SCR_REGISTER_SECURE(PLAY_ANIM_ON_RUNNING_SCENARIO, 0x8b90895f7d0bee59, CommandPlayAnimOnRunnningScenario);

    SCR_REGISTER_SECURE(DOES_SCENARIO_GROUP_EXIST, 0x1e63be2394148da0, CommandDoesScenarioGroupExist);
    SCR_REGISTER_SECURE(IS_SCENARIO_GROUP_ENABLED, 0x96a05000ccd43584, CommandIsScenarioGroupEnabled);
    SCR_REGISTER_SECURE(SET_SCENARIO_GROUP_ENABLED, 0xa3d2c191df3cb742, CommandSetScenarioGroupEnabled);
    SCR_REGISTER_SECURE(RESET_SCENARIO_GROUPS_ENABLED, 0xb750df5029a6790a, CommandResetScenarioGroupsEnabled);
    SCR_REGISTER_SECURE(SET_EXCLUSIVE_SCENARIO_GROUP, 0xedf1af96b1a92325, CommandSetExclusiveScenarioGroup);
    SCR_REGISTER_SECURE(RESET_EXCLUSIVE_SCENARIO_GROUP, 0xa7fee431b0d22e80, CommandResetExclusiveScenarioGroup);
    SCR_REGISTER_UNUSED(FORCE_SCENARIO_GROUP_PRIORITY, 0xb2a385f314986d7d, CommandForceScenarioGroupPriority);
    SCR_REGISTER_UNUSED(RESET_SCENARIO_GROUPS_PRIORITY, 0xf842c44cfffb8884, CommandResetScenarioGroupsPriority);

    SCR_REGISTER_SECURE(IS_SCENARIO_TYPE_ENABLED, 0xaec3fb024079cf27, CommandIsScenarioTypeEnabled);
    SCR_REGISTER_SECURE(SET_SCENARIO_TYPE_ENABLED, 0x247f21b1803f0ec4, CommandSetScenarioTypeEnabled);
    SCR_REGISTER_SECURE(RESET_SCENARIO_TYPES_ENABLED, 0xa9fc42dea687033d, CommandResetScenarioTypesEnabled);

    SCR_REGISTER_UNUSED(SUPPRESS_NORMAL_SCENARIO_EXITS_NEXT_FRAME, 0x462559c9bfdb51d1, CommandSuppressNormalScenarioExitsNextFrame);
    SCR_REGISTER_UNUSED(SUPPRESS_SCENARIO_ATTRACTION_NEXT_FRAME, 0xf50d4dd468864fca, CommandSuppressScenarioAttractionNextFrame);
    SCR_REGISTER_UNUSED(SUPPRESS_BREAKOUT_SCENARIO_EXITS_NEXT_FRAME, 0x743ef6f4b8c462a8, CommandSuppressBreakoutScenarioExitsNextFrame);

    SCR_REGISTER_SECURE(IS_PED_ACTIVE_IN_SCENARIO, 0x67360b9474054c5e, CommandIsPedActiveInScenario);
    SCR_REGISTER_SECURE(IS_PED_PLAYING_BASE_CLIP_IN_SCENARIO, 0x2dc1bc483b6a3316, CommandIsPedPlayingBaseClipInScenario);
    SCR_REGISTER_SECURE(SET_PED_CAN_PLAY_AMBIENT_IDLES, 0xe72d9eb9db4b874d, CommandSetCanPlayAmbientIdles);

    SCR_REGISTER_SECURE(TASK_COMBAT_HATED_TARGETS_IN_AREA, 0xe45039a194d735de, CommandTaskCombatHatedTargetsInArea);
    SCR_REGISTER_SECURE(TASK_COMBAT_HATED_TARGETS_AROUND_PED, 0x766d377a16f499e3, CommandTaskCombatHatedTargetsAroundPed);
    SCR_REGISTER_SECURE(TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED, 0x34ad04c1e7cb67d8, CommandTaskCombatHatedTargetsAroundPedTimed);

    SCR_REGISTER_SECURE(TASK_THROW_PROJECTILE, 0xa43ab969c8956eb8, CommandTaskThrowProjectile);

    SCR_REGISTER_SECURE(TASK_SWAP_WEAPON, 0xd4834169f570e6d2, CommandTaskSwapWeapon);
    SCR_REGISTER_SECURE(TASK_RELOAD_WEAPON, 0x62f1ebed7a072b40, CommandTaskReloadWeapon);
    SCR_REGISTER_UNUSED(TASK_COMBAT_ROLL, 0x8eddf98bab0a7bc9, CommandTaskCombatRoll);
    SCR_REGISTER_SECURE(IS_PED_GETTING_UP, 0x03454821a9ecf4aa, CommandIsPedGettingUp);
    SCR_REGISTER_SECURE(TASK_WRITHE, 0x99e9c3c2d6af9a52, CommandTaskWrithe);
    SCR_REGISTER_SECURE(IS_PED_IN_WRITHE, 0x645f244ecddef6cf, CommandIsPedInWrithe);

    SCR_REGISTER_SECURE(OPEN_PATROL_ROUTE, 0x9d3e7062e40f2049, CommandOpenPatrolRoute);
    SCR_REGISTER_SECURE(CLOSE_PATROL_ROUTE, 0xc65dee4d8f9f02eb, CommandClosePatrolRoute);
    SCR_REGISTER_SECURE(ADD_PATROL_ROUTE_NODE, 0xad84831de60c1442, CommandAddPatrolNode);
    SCR_REGISTER_SECURE(ADD_PATROL_ROUTE_LINK, 0x23c767cf5993cf13, CommandAddPatrolNodeLink);
    SCR_REGISTER_SECURE(CREATE_PATROL_ROUTE, 0x274f700901c096dd, CommandCreatePatrolRoute);
    SCR_REGISTER_SECURE(DELETE_PATROL_ROUTE, 0x6bd70dca1fdc1c56, CommandDeletePatrolRoute);
    SCR_REGISTER_SECURE(GET_PATROL_TASK_INFO, 0x52f734cebe20dfba, CommandGetPatrolNodeInfo);

    SCR_REGISTER_SECURE(TASK_PATROL, 0xac81ed4a4f2fec9c, CommandTaskPatrol);

    SCR_REGISTER_SECURE(TASK_STAY_IN_COVER, 0xf21f1b3825b46822, CommandTaskStayInCover);

    SCR_REGISTER_SECURE(ADD_VEHICLE_SUBTASK_ATTACK_COORD, 0xf2e8f4bd20869ab2, CommandAddVehicleSubtaskAttackCoord);
    SCR_REGISTER_SECURE(ADD_VEHICLE_SUBTASK_ATTACK_PED, 0x80e8bfe6e62212c1, CommandAddVehicleSubtaskAttackPed);

    SCR_REGISTER_SECURE(TASK_VEHICLE_SHOOT_AT_PED, 0xbba695ad5315cdd8, CommandVehicleShootAtPed);
    SCR_REGISTER_SECURE(TASK_VEHICLE_AIM_AT_PED, 0xc514d16960a4fa43, CommandVehicleAimAtPed);
    SCR_REGISTER_SECURE(TASK_VEHICLE_SHOOT_AT_COORD, 0xe5d96c43ab0b0dc6, CommandVehicleShootAtCoord);
    SCR_REGISTER_SECURE(TASK_VEHICLE_AIM_AT_COORD, 0x7d49955a299139e9, CommandVehicleAimAtCoord);
    SCR_REGISTER_UNUSED(TASK_VEHICLE_AIM_USING_CAMERA, 0x6614a146419eb189, CommandVehicleAimUsingCamera);
    SCR_REGISTER_UNUSED(TASK_HANG_GLIDER, 0xbdbadc2ff688e5b2, CommandTaskHangGlider);
    SCR_REGISTER_UNUSED(SET_HANG_GLIDER_AIR_SPEED, 0x23b2c655efd82c53, CommandSetHangGliderAirSpeed);
    SCR_REGISTER_SECURE(TASK_VEHICLE_GOTO_NAVMESH, 0xdf44785f7bc09dd2, CommandTaskVehicleGotoNavmesh);

    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD, 0x642c58f4ca259448, CommandTaskGoToCoordWhileAimingAtCoord);
    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY, 0x98db2abd53b8d70b, CommandTaskGoToCoordWhileAimingAtEntity);
    SCR_REGISTER_SECURE(TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD, 0x2a83cd9941ec44d0, CommandTaskGoToCoordAndAimAtHatedEntitiesNearCoord);
    SCR_REGISTER_SECURE(TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD, 0x60fad147bb45a92a, CommandTaskGoToEntityWhileAimingAtCoord);
    SCR_REGISTER_SECURE(TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY, 0x0b47872e9d74882c, CommandTaskGoToEntityWhileAimingAtEntity);

    SCR_REGISTER_UNUSED(TASK_BIND_POSE, 0xac5fea0754bcc7cf, CommandTaskBindPose);
    SCR_REGISTER_UNUSED(ELECTROCUTE_PED, 0x9eebf0c360e75bef, CommandElectrocute);
    SCR_REGISTER_SECURE(SET_HIGH_FALL_TASK, 0xdb1174c6546afc65, CommandHighFall);
    SCR_REGISTER_UNUSED(DANGLE_FROM_MEATHOOK, 0xd9c1a873ff69a447, CommandDangleFromMeathook);
    SCR_REGISTER_UNUSED(SLUNG_OVER_SHOULDER, 0xcbdd75b6dfb92714, CommandSlungOverShoulder);
    SCR_REGISTER_UNUSED(MAKE_PED_STUMBLE, 0x6b0ebf2507baba30, CommandStumble);

    SCR_REGISTER_SECURE(REQUEST_WAYPOINT_RECORDING, 0x70f260358d1a42c4, waypoint_commands::CommandRequestWaypointRecording);
    SCR_REGISTER_SECURE(GET_IS_WAYPOINT_RECORDING_LOADED, 0xbd3cea9cd36e271e, waypoint_commands::CommandGetIsWaypointRecordingLoaded);
    SCR_REGISTER_SECURE(REMOVE_WAYPOINT_RECORDING, 0x10f7bfe3a88cb99b, waypoint_commands::CommandRemoveWaypointRecording);

    SCR_REGISTER_SECURE(WAYPOINT_RECORDING_GET_NUM_POINTS, 0x042959494b85e155, waypoint_commands::CommandWaypointRecordingGetNumPoints);
    SCR_REGISTER_SECURE(WAYPOINT_RECORDING_GET_COORD, 0x76263b6f2b150aff, waypoint_commands::CommandWaypointRecordingGetCoord);
    SCR_REGISTER_SECURE(WAYPOINT_RECORDING_GET_SPEED_AT_POINT, 0xaa4b93a8c0d05be3, waypoint_commands::CommandWaypointRecordingGetSpeedAtPoint);
    SCR_REGISTER_SECURE(WAYPOINT_RECORDING_GET_CLOSEST_WAYPOINT, 0xe49fae8c75dfe3b0, waypoint_commands::CommandWaypointRecordingGetClosestWaypoint);

    SCR_REGISTER_SECURE(TASK_FOLLOW_WAYPOINT_RECORDING, 0x4218764824f31173, waypoint_commands::CommandTaskFollowWaypointRecording);
    SCR_REGISTER_SECURE(IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_PED, 0xb4ae2a423a4176ce, waypoint_commands::CommandIsWaypointPlaybackGoingOnForPed);
    SCR_REGISTER_SECURE(GET_PED_WAYPOINT_PROGRESS, 0x0b331f8dc5c61933, waypoint_commands::CommandGetPedWaypointProgress);
    SCR_REGISTER_UNUSED(SET_PED_WAYPOINT_PROGRESS, 0xadc4c24903027450, waypoint_commands::CommandSetPedWaypointProgress);
    SCR_REGISTER_SECURE(GET_PED_WAYPOINT_DISTANCE, 0xb80b2f3bfb68df1a, waypoint_commands::CommandGetPedWaypointDistance);
    SCR_REGISTER_SECURE(SET_PED_WAYPOINT_ROUTE_OFFSET, 0x455e0249f399ef4c, waypoint_commands::CommandSetPedWaypointRouteOffset);
    SCR_REGISTER_SECURE(GET_WAYPOINT_DISTANCE_ALONG_ROUTE, 0xd2fe51a38e9a1946, waypoint_commands::CommandGetWaypointDistanceAlongRoute);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_GET_IS_PAUSED, 0x656f70678f75d966, waypoint_commands::CommandWaypointPlaybackGetIsPaused);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_PAUSE, 0x499c4b0d70f4097c, waypoint_commands::CommandWaypointPlaybackPause);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_RESUME, 0x4ffeb98e621afb27, waypoint_commands::CommandWaypointPlaybackResume);

    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_OVERRIDE_SPEED, 0x369b082b04a4f619, waypoint_commands::CommandWaypointPlaybackOverrideSpeed);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED, 0x5039c5050b42f1e0, waypoint_commands::CommandWaypointPlaybackUseDefaultSpeed);

    SCR_REGISTER_SECURE(USE_WAYPOINT_RECORDING_AS_ASSISTED_MOVEMENT_ROUTE, 0xbb6589e0d27bd54e, waypoint_commands::CommandUseWaypointRecordingAsAssistedMovementRoute);

    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_START_AIMING_AT_PED, 0x9b4dd143cd0c78fe, waypoint_commands::CommandWaypointPlaybackStartAimingAtPed);
    SCR_REGISTER_UNUSED(WAYPOINT_PLAYBACK_START_AIMING_AT_ENTITY, 0x47e2d6d68a34062a, waypoint_commands::CommandWaypointPlaybackStartAimingAtEntity);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_START_AIMING_AT_COORD, 0xa142fd669ff80016, waypoint_commands::CommandWaypointPlaybackStartAimingAtCoord);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_START_SHOOTING_AT_PED, 0xa9b13da1d075a160, waypoint_commands::CommandWaypointPlaybackStartShootingAtPed);
    SCR_REGISTER_UNUSED(WAYPOINT_PLAYBACK_START_SHOOTING_AT_ENTITY, 0x4f6e75c1ca97a1ef, waypoint_commands::CommandWaypointPlaybackStartShootingAtEntity);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_START_SHOOTING_AT_COORD, 0x4f826adf9bdb6bd8, waypoint_commands::CommandWaypointPlaybackStartShootingAtCoord);
    SCR_REGISTER_SECURE(WAYPOINT_PLAYBACK_STOP_AIMING_OR_SHOOTING, 0x282cae6fb0e3b2fc, waypoint_commands::CommandWaypointPlaybackStopAimingOrShooting);

    SCR_REGISTER_SECURE(ASSISTED_MOVEMENT_REQUEST_ROUTE, 0xb5549ea4fcf75870, waypoint_commands::CommandAssistedMovementRequestRoute);
    SCR_REGISTER_SECURE(ASSISTED_MOVEMENT_REMOVE_ROUTE, 0xed4af6b9266b55c9, waypoint_commands::CommandAssistedMovementRemoveRoute);

    SCR_REGISTER_SECURE(ASSISTED_MOVEMENT_IS_ROUTE_LOADED, 0x632b7d3b9b85016a, waypoint_commands::CommandAssistedMovementIsRouteLoaded);
    SCR_REGISTER_UNUSED(ASSISTED_MOVEMENT_GET_ROUTE_PROPERTIES, 0xec6dfa1a86cf270a, waypoint_commands::CommandAssistedMovementGetRouteProperties);
    SCR_REGISTER_SECURE(ASSISTED_MOVEMENT_SET_ROUTE_PROPERTIES, 0x50569e4320bc8e92, waypoint_commands::CommandAssistedMovementSetRouteProperties);
    SCR_REGISTER_SECURE(ASSISTED_MOVEMENT_OVERRIDE_LOAD_DISTANCE_THIS_FRAME, 0xd7e4e11cc0659bb6, waypoint_commands::CommandAssistedMovementOverrideLoadDistanceThisFrame);

    SCR_REGISTER_SECURE(TASK_VEHICLE_FOLLOW_WAYPOINT_RECORDING, 0xa24707688ba83cea, waypoint_commands::CommandTaskVehicleFollowWaypointRecording);
    SCR_REGISTER_SECURE(IS_WAYPOINT_PLAYBACK_GOING_ON_FOR_VEHICLE, 0x22ff01f89d3006c0, waypoint_commands::CommandIsWaypointPlaybackGoingOnForVehicle);
    SCR_REGISTER_SECURE(GET_VEHICLE_WAYPOINT_PROGRESS, 0x46ea6598acefacc1, waypoint_commands::CommandGetVehicleWaypointProgress);
    SCR_REGISTER_SECURE(GET_VEHICLE_WAYPOINT_TARGET_POINT, 0x0e9d3608e0ae9ee3, waypoint_commands::CommandGetVehicleWaypointTargetPoint);
    SCR_REGISTER_SECURE(VEHICLE_WAYPOINT_PLAYBACK_PAUSE, 0xe3948de25c763434, waypoint_commands::CommandVehicleWaypointPlaybackPause);
    SCR_REGISTER_UNUSED(VEHICLE_WAYPOINT_PLAYBACK_GET_IS_PAUSED, 0xa84a1cf14183e169, waypoint_commands::CommandVehicleWaypointPlaybackGetIsPaused);
    SCR_REGISTER_SECURE(VEHICLE_WAYPOINT_PLAYBACK_RESUME, 0x211bb80656beac0f, waypoint_commands::CommandVehicleWaypointPlaybackResume);
    SCR_REGISTER_SECURE(VEHICLE_WAYPOINT_PLAYBACK_USE_DEFAULT_SPEED, 0x502f9fce448f5724, waypoint_commands::CommandVehicleWaypointPlaybackUseDefaultSpeed);
    SCR_REGISTER_SECURE(VEHICLE_WAYPOINT_PLAYBACK_OVERRIDE_SPEED, 0xf248aeaab65d17cd, waypoint_commands::CommandVehicleWaypointPlaybackOverrideSpeed);

    SCR_REGISTER_UNUSED(TASK_NM_ATTACH_PED_TO_ENTITY, 0x7ed90e7321ac4a87, CommandTaskNMAttachPedToEntity);
    SCR_REGISTER_SECURE(TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS, 0xf7b73727a8f72f54, CommandTaskSetBlockingOfNonTemporaryEvents);
    SCR_REGISTER_SECURE(TASK_FORCE_MOTION_STATE, 0x57374b880e0a67bd, CommandTaskForceMotionState);

    // MoVE interface
    SCR_REGISTER_SECURE(TASK_MOVE_NETWORK_BY_NAME, 0x84a70ae1a2393914, CommandTaskMoveNetworkByName);
    SCR_REGISTER_SECURE(TASK_MOVE_NETWORK_ADVANCED_BY_NAME, 0xad3d17c7e426d09d, CommandTaskMoveNetworkAdvancedByName);
    SCR_REGISTER_SECURE(TASK_MOVE_NETWORK_BY_NAME_WITH_INIT_PARAMS, 0x4601817fd87b48e3, CommandTaskMoveNetworkByNameWithInitParams);
    SCR_REGISTER_SECURE(TASK_MOVE_NETWORK_ADVANCED_BY_NAME_WITH_INIT_PARAMS, 0xca9cbdbc10f9d281, CommandTaskMoveNetworkAdvancedByNameWithInitParams);
    SCR_REGISTER_SECURE(IS_TASK_MOVE_NETWORK_ACTIVE, 0xfcd53f51d587f4c6, CommandIsTaskMoveNetworkActive);
    SCR_REGISTER_SECURE(IS_TASK_MOVE_NETWORK_READY_FOR_TRANSITION, 0x0422aefd9002867b, CommandIsTaskMoveNetworkReadyForTransition);
    SCR_REGISTER_SECURE(REQUEST_TASK_MOVE_NETWORK_STATE_TRANSITION, 0xdcd29be535921736, CommandRequestTaskMoveNetworkStateTransition);
    SCR_REGISTER_SECURE(SET_EXPECTED_CLONE_NEXT_TASK_MOVE_NETWORK_STATE, 0x2029bef342249fb1, CommandSetExpectedCloneNextTaskMoveNetworkState);
    SCR_REGISTER_SECURE(GET_TASK_MOVE_NETWORK_STATE, 0x111890991356db66, CommandGetTaskMoveNetworkState);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_ANIM_SET, 0x2d9f06756211492a, CommandSetTaskMoveNetworkClipSet);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT, 0x8e11f085404a3100, CommandSetTaskMoveNetworkSignalFloat);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_FLOAT, 0x357c016ef8883bad, CommandSetTaskMoveNetworkSignalLocalFloat);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_SIGNAL_FLOAT_LERP_RATE, 0x6e3f82dd454ea5b1, CommandSetTaskMoveNetworkSignalFloatLerpRate);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_SIGNAL_BOOL, 0xdde2a56a49baf5b0, CommandSetTaskMoveNetworkSignalBool);
    SCR_REGISTER_UNUSED(SET_TASK_MOVE_NETWORK_SIGNAL_LOCAL_BOOL, 0xbe46dff5c216a2b0, CommandSetTaskMoveNetworkSignalLocalBool);
    SCR_REGISTER_SECURE(GET_TASK_MOVE_NETWORK_SIGNAL_FLOAT, 0x103bce39f925945e, CommandGetTaskMoveNetworkSignalFloat);
    SCR_REGISTER_SECURE(GET_TASK_MOVE_NETWORK_SIGNAL_BOOL, 0x501632a36a6512aa, CommandGetTaskMoveNetworkSignalBool);
    SCR_REGISTER_SECURE(GET_TASK_MOVE_NETWORK_EVENT, 0xce3f029a4833b5e1, CommandGetTaskMoveNetworkEvent);
    SCR_REGISTER_SECURE(SET_TASK_MOVE_NETWORK_ENABLE_COLLISION_ON_NETWORK_CLONE_WHEN_FIXED, 0x4a5f1661be58e97e, CommandSetTaskMoveNetworkEnableCollisionOnNetworkCloneWhenFixed);
    // End MoVE interface

    SCR_REGISTER_SECURE(IS_MOVE_BLEND_RATIO_STILL, 0x98daeb6e7093895e, CommandIsMoveBlendRatioStill);
    SCR_REGISTER_SECURE(IS_MOVE_BLEND_RATIO_WALKING, 0x6b46f0285c770b5b, CommandIsMoveBlendRatioWalking);
    SCR_REGISTER_SECURE(IS_MOVE_BLEND_RATIO_RUNNING, 0x833a52f892a42353, CommandIsMoveBlendRatioRunning);
    SCR_REGISTER_SECURE(IS_MOVE_BLEND_RATIO_SPRINTING, 0x3861da7f20602340, CommandIsMoveBlendRatioSprinting);
    SCR_REGISTER_SECURE(IS_PED_STILL, 0x443c6afb940b3c83, CommandIsPedStill);
    SCR_REGISTER_SECURE(IS_PED_WALKING, 0x834c649d79d5128d, CommandIsPedWalking);
    SCR_REGISTER_SECURE(IS_PED_RUNNING, 0x81bbcaf9fe3b469d, CommandPedIsRunning);
    SCR_REGISTER_SECURE(IS_PED_SPRINTING, 0xe02d8bdf4b18d48d, CommandPedIsSprinting);
    SCR_REGISTER_SECURE(IS_PED_STRAFING, 0xfe3baaf47e48d31f, CommandPedIsStrafing);

    // synchronized scenes
    SCR_REGISTER_SECURE(TASK_SYNCHRONIZED_SCENE, 0x950b26f4c891073f, CommandTaskSynchronizedScene);

    SCR_REGISTER_SECURE(TASK_AGITATED_ACTION_CONFRONT_RESPONSE, 0x3e7d91f4eece76b8, CommandGivePedAgitatedTaskConfront);

    //////////////////////////////////////////////////////////////////////////
    //	Ped SweepTask commands
    //////////////////////////////////////////////////////////////////////////
    SCR_REGISTER_SECURE(TASK_SWEEP_AIM_ENTITY, 0x26d2934cf2d5b32c, CommandTaskSweepAimEntity);
    SCR_REGISTER_SECURE(UPDATE_TASK_SWEEP_AIM_ENTITY, 0x878b7bb84cc31442, CommandUpdateTaskSweepAimEntity);
    SCR_REGISTER_SECURE(TASK_SWEEP_AIM_POSITION, 0x07c9c36e1a3234e9, CommandTaskSweepAimPosition);
    SCR_REGISTER_SECURE(UPDATE_TASK_SWEEP_AIM_POSITION, 0xc82e4267a5f18820, CommandUpdateTaskSweepAimPosition);

    // @DEPRECATED: Old arrest commands
    SCR_REGISTER_SECURE(TASK_ARREST_PED, 0x463d4f9c4826a59b, CommandTaskArrestPed);
    SCR_REGISTER_SECURE(IS_PED_RUNNING_ARREST_TASK, 0x64152dd440e37f50, CommandIsPedRunningArrestTask);
    SCR_REGISTER_UNUSED(IS_ARREST_TYPE_VALID, 0x1c805a9f43140ae6, CommandIsArrestTypeValid);

    // Arrest/uncuff commands
    SCR_REGISTER_SECURE(IS_PED_BEING_ARRESTED, 0x1c7f03843c4541ad, CommandIsPedBeingArrested);
    SCR_REGISTER_UNUSED(IS_PED_BEING_UNCUFFED, 0x522ef0aaa44cd03a, CommandIsPedBeingUncuffed);
    SCR_REGISTER_UNUSED(IS_PED_ARRESTING, 0x2a706f5c5d386b6e, CommandIsPedArresting);
    SCR_REGISTER_UNUSED(IS_PED_UNCUFFING, 0x1590a589bdbbf96c, CommandIsPedUncuffing);
    SCR_REGISTER_UNUSED(GET_PED_ARRESTER, 0x1d081ef24f8c6203, CommandGetPedArrester);
    SCR_REGISTER_UNUSED(GET_PED_UNCUFFER, 0x79a3054bf21c9039, CommandGetPedUncuffer);
    SCR_REGISTER_UNUSED(GET_PED_ARRESTING_TARGET, 0x9def9d10068f6f46, CommandGetPedArrestingTarget);
    SCR_REGISTER_UNUSED(GET_PED_UNCUFFING_TARGET, 0x08c5c742c9b17f90, CommandGetPedUncuffingTarget);

    SCR_REGISTER_UNUSED(CUFF_PED, 0x7b9873a2f170f8b0, CommandCuffPed);
    SCR_REGISTER_SECURE(UNCUFF_PED, 0x9a89b72b9931965c, CommandUncuffPed);
    SCR_REGISTER_SECURE(IS_PED_CUFFED, 0xea266e5f12328e62, CommandIsPedCuffed);

    SCR_REGISTER_UNUSED(IS_PED_TAKING_CUSTODY, 0xb178acc0bca5b82c, CommandIsPedTakingCustody);
    SCR_REGISTER_UNUSED(GET_PED_TAKING_CUSTODY_TARGET, 0x2f6461d574428769, CommandGetPedTakingCustodyTarget);
    SCR_REGISTER_UNUSED(IS_PED_BEING_TAKEN_INTO_CUSTODY, 0xcaff880c04b723c0, CommandIsPedBeingTakenIntoCustody);

    SCR_REGISTER_UNUSED(TASK_TAKE_CUSTODY, 0x53baad9817f5de01, CommandTaskTakeCustody);
    SCR_REGISTER_UNUSED(GET_PED_CUSTODIAN, 0x670b52c53b47a811, CommandGetPedCustodian);
    SCR_REGISTER_UNUSED(REMOVE_PED_FROM_CUSTODY, 0x512b985df2d0e125, CommandRemovePedFromCustody);

    SCR_REGISTER_UNUSED(GET_PED_ARREST_PHASE, 0x44559d9a1dd62526, CommandGetPedArrestPhase);

    SCR_REGISTER_UNUSED(GET_PED_POTENTIAL_ARREST_TARGET, 0x1214edf270c95469, CommandGetPedPotentialArrestTarget);
    SCR_REGISTER_UNUSED(GET_PED_POTENTIAL_UNCUFF_TARGET, 0x346aeaa1e63abaeb, CommandGetPedPotentialUncuffTarget);

    SCR_REGISTER_UNUSED(SET_PED_CUSTODY_FOLLOW_DISTANCE, 0x006f134efa946bd1, CommandSetPedCustodyOverrideFollowDistance);
    SCR_REGISTER_UNUSED(WARP_INTO_LEADERS_VEHICLE, 0x196931342f1803a2, CommandWarpIntoLeadersVehicle);
  }
}  //	end of namespace task_commands