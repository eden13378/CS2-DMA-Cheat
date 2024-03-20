#include "Radar.h"
#include "D3DX11tex.h"
#include "D3DX11core.h"

#include "../mapsdata.h"

void Base_Radar::UpdateMap(std::string mapname) {
	if (mapname != mp::current_map_name) {
		mp::current_map_name = mapname;
		if (mapname == "<empty>") {
			mp::map_texture = NULL;
			return;
		}
		std::cout << "[ log ] Changing map to " << mapname << std::endl;
		auto found_map = mp::maps_data.find(mapname);
		if (found_map == mp::maps_data.end()) {
			std::cout << "[ log ] could not find map data!" << std::endl;
		}
		else {
			mp::map_zoom = found_map->second[0];
			mp::map_offset_x = found_map->second[1];
			mp::map_offset_y = found_map->second[2];
			if (mp::current_map_name == "de_nuke" || mp::current_map_name == "de_vertigo") {
				mp::split_z = found_map->second[3];
				mp::split_x = found_map->second[4];
				mp::split_y = found_map->second[5];
			}
		}
		D3DX11_IMAGE_LOAD_INFO info_text; ID3DX11ThreadPump* pump_text{ nullptr };

		if (mp::current_map_name == "de_mirage") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_mirage, sizeof(mp::de_mirage), &info_text, pump_text, &mp::map_texture, 0);
		} else if (mp::current_map_name == "de_overpass") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_overpass, sizeof(mp::de_overpass), &info_text, pump_text, &mp::map_texture, 0);
		}
		else if (mp::current_map_name == "de_inferno") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_inferno, sizeof(mp::de_inferno), &info_text, pump_text, &mp::map_texture, 0);
		}
		else if (mp::current_map_name == "de_anubis") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_anubis, sizeof(mp::de_anubis), &info_text, pump_text, &mp::map_texture, 0);
		}
		else if (mp::current_map_name == "de_ancient") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_ancient, sizeof(mp::de_ancient), &info_text, pump_text, &mp::map_texture, 0);
		} else if (mp::current_map_name == "de_dust2") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_dust2_new, sizeof(mp::de_dust2_new), &info_text, pump_text, &mp::map_texture, 0);
		}
		else if (mp::current_map_name == "de_nuke") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_nuke, sizeof(mp::de_nuke), &info_text, pump_text, &mp::map_texture, 0);
		}
		else if (mp::current_map_name == "de_vertigo") {
			D3DX11CreateShaderResourceViewFromMemory(Gui.getDevice(), mp::de_vertigo, sizeof(mp::de_vertigo), &info_text, pump_text, &mp::map_texture, 0);
		}
		delete pump_text;
	}
}


std::pair<int, int> Base_Radar::rotate_point(const std::pair<int, int>& center, const std::pair<int, int>& point, double angle) {
	double angle_rad = angle * M_PI / 180.0;
	std::pair<double, double> temp_point = std::make_pair(point.first - center.first, center.second - point.second);
	temp_point = std::make_pair(
		temp_point.first * cos(angle_rad) - temp_point.second * sin(angle_rad),
		temp_point.first * sin(angle_rad) + temp_point.second * cos(angle_rad)
	);
	temp_point = std::make_pair(temp_point.first + center.first, center.second - temp_point.second);
	return std::make_pair(static_cast<int>(temp_point.first), static_cast<int>(temp_point.second));
}

std::pair<int, int> Base_Radar::world_to_minimap2(double x, double y, double pos_x, double pos_y, double scale, int map_w, int map_h) {
	int image_x = static_cast<int>((x - pos_x) * mp::RadarSize / (map_w * scale * 2));
	int image_y = static_cast<int>((y - pos_y) * mp::RadarSize / (map_h * scale * 2));
	std::pair<int, int> center = std::make_pair(mp::RadarSize / 2, mp::RadarSize / 2);
	auto rotated_point = rotate_point(center, std::make_pair(image_x, image_y), 0);
	return rotated_point;
}


void Base_Radar::SetRange(const float& Range)
{
	this->RenderRange = Range;
}

void Base_Radar::SetCrossColor(const ImColor& Color)
{
	this->CrossColor = Color;
}

void Base_Radar::SetPos(const Vec2& Pos)
{
	this->Pos = Pos;
}

void Base_Radar::SetSize(const float& Size)
{
	this->Width = Size;
}

float Base_Radar::GetSize()
{
	return this->Width;
}

Vec2 Base_Radar::GetPos()
{
	return this->Pos;
}

void Base_Radar::SetProportion(const float& Proportion)
{
	this->Proportion = Proportion;
}

void Base_Radar::SetDrawList(ImDrawList* DrawList)
{
	this->DrawList = DrawList;
}

void Base_Radar::AddPoint(const Vec3& LocalPos, const float& LocalYaw, const Vec3& Pos, ImColor Color, float Yaw, int same_team, float hp, int is_bomb)
{
	std::vector<std::pair<int, float>> insert_data;

	std::pair <int, int> resolved;
	Vec2 PointPos;
	if (mp::current_map_name == "de_nuke" || mp::current_map_name == "de_vertigo") {
		if (Pos.z < mp::split_z) resolved = this->world_to_minimap2(Pos.y, Pos.x, mp::split_x, mp::split_y, mp::map_zoom, mp::img_h, mp::img_w);
		else resolved = this->world_to_minimap2(Pos.y, Pos.x, mp::map_offset_x, mp::map_offset_y, mp::map_zoom, mp::img_h, mp::img_w);
	}
	else {
		resolved = this->world_to_minimap2(Pos.y, Pos.x, mp::map_offset_x, mp::map_offset_y, mp::map_zoom, mp::img_h, mp::img_w);
	}
	PointPos.x = resolved.first;
	PointPos.y = resolved.second;
	insert_data.push_back(std::make_pair(same_team, LocalYaw));
	insert_data.push_back(std::make_pair(hp, is_bomb));
	this->Points_new.push_back(std::make_pair(PointPos, insert_data));

}


void Base_Radar::Render()
{
	this->DrawList->AddImage((void*)mp::map_texture, ImVec2(this->Pos.x, this->Pos.y), ImVec2(this->Pos.x + mp::RadarSize, this->Pos.y + mp::RadarSize));
	this->DrawList->AddRectFilled(this->Pos.ToImVec2(), ImVec2(this->Pos.x + mp::RadarSize, this->Pos.y + mp::RadarSize), ImColor(255, 255, 255, 10));

	ImColor color_triangle;
	ImColor color_point;

	for (auto PointSingle : this->Points_new) {
		if (PointSingle.second[0].first == 1) {
			color_point = ImColor(0, 255, 0, 255);
			color_triangle = ImColor(0, 255, 0, 255);
		}
		else if (PointSingle.second[0].first == 0) {
			color_point = ImColor(255, 0, 0, 255);
			ImVec2 text_pos(PointSingle.first.ToImVec2().x - 10, PointSingle.first.ToImVec2().y + 2);
			this->DrawList->AddText(ImVec2(this->Pos.x + text_pos.x, this->Pos.y + text_pos.y), ImColor(0, 255, 0, 255), std::to_string(PointSingle.second[1].first).c_str());
			color_triangle = ImColor(255, 0, 0, 255);
		}
		else if (PointSingle.second[0].first == 2) {
			color_point = ImColor(0, 226, 210, 255);
			color_triangle = ImColor(0, 226, 210, 255);
		}
		float radian = PointSingle.second[0].second * (M_PI / 180);

		float triangle_top_x = PointSingle.first.ToImVec2().x + this->Pos.x + sin(radian) * mp::LineLenght;
		float triangle_top_y = PointSingle.first.ToImVec2().y + this->Pos.y + cos(radian) * mp::LineLenght;
		float triangle_left_x = PointSingle.first.ToImVec2().x + this->Pos.x + sin(radian + M_PI / 3) * mp::LineLenght / 2;
		float triangle_left_y = PointSingle.first.ToImVec2().y + this->Pos.y + cos(radian + M_PI / 3) * mp::LineLenght / 2;

		float triangle_right_x = PointSingle.first.ToImVec2().x + this->Pos.x + sin(radian - M_PI / 3) * mp::LineLenght / 2;
		float triangle_right_y = PointSingle.first.ToImVec2().y + this->Pos.y + cos(radian - M_PI / 3) * mp::LineLenght / 2;

		Vec2 x1 = Vec2(triangle_top_x, triangle_top_y);
		Vec2 x2 = Vec2(triangle_left_x, triangle_left_y);
		Vec2 x3 = Vec2(triangle_right_x, triangle_right_y);
		this->DrawList->AddCircle(ImVec2(PointSingle.first.x + this->Pos.x, PointSingle.first.y + this->Pos.y), mp::CircleSize, color_point, 0, 5);

		this->DrawList->AddTriangle(x2.ToImVec2(), x3.ToImVec2(), x1.ToImVec2(), color_triangle, 3);
	}
	if (this->Points_new.size() > 0)
		this->Points_new.clear();
}