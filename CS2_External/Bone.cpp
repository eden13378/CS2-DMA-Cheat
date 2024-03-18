#include "Bone.h"

bool CBone::UpdateAllBoneData(const DWORD64& EntityPawnAddress)
{
	if (EntityPawnAddress == 0)
		return false;
	this->EntityPawnAddress = EntityPawnAddress;

	DWORD64 GameSceneNode = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnAddress + Offset::GameSceneNode, GameSceneNode))
		return false;
	if (!ProcessMgr.ReadMemory<DWORD64>(GameSceneNode + Offset::BoneArray, this->BoneArrayAddress))
		return false;

	BoneJointData BoneArray[30]{};
	if (!ProcessMgr.ReadMemory(this->BoneArrayAddress, BoneArray, 30 * sizeof(BoneJointData)))
		return false;
	this->BonePosList.clear();
	for (int i = 0; i < 30; i++)
	{
		Vec2 ScreenPos;
		bool IsVisible = false;

		if (gGame.View.WorldToScreen(BoneArray[i].Pos, ScreenPos))
			IsVisible = true;

		this->BonePosList.push_back({ BoneArray[i].Pos ,ScreenPos,IsVisible });
	}

	return this->BonePosList.size() > 0;
}