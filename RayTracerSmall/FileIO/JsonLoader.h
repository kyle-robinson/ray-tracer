#pragma once
#ifndef JSONLOADER_H
#define JSONLOADER_H

#include "JsonData.h"
#include "../Resources/json.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class JsonLoader
{
public:
	static JsonData* LoadSphereInfo( const char* filepath );
private:
	static bool HasAttribute( nlohmann::json* file, const std::string& key );
	static Vec3f LoadVec3f( const std::vector<float>& vec );
};

#endif