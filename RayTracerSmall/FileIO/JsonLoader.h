#pragma once
#ifndef JSONLOADER_H
#define JSONLOADER_H

#include "json.hpp"
#include "JsonData.h"

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