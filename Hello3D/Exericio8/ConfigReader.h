#pragma once

#include <string>

#include <iostream>
//#include <json/value.h>
//#include <json/json.h>
//#include <jsoncpp/json/json.h>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Camera
{
    std::string xPos;
    std::string yPos;
    std::string zPos;
};

struct Illumination
{
    std::string strength;
};

struct SceneObject
{
    std::string filePath;
    std::string scale;
    std::string xDeslocation;
};

struct Configuration
{
    Camera camera;
    Illumination illumination;
    std::vector<SceneObject> sceneObjects;
};

void from_json(const json& j, Configuration& configuration)
{
    Camera camera;
    j.at("camera").at("xPos").get_to(camera.xPos);
    j.at("camera").at("yPos").get_to(camera.yPos);
    j.at("camera").at("zPos").get_to(camera.zPos);
    configuration.camera = camera;

    Illumination illumination;
    j.at("illumination").at("strength").get_to(illumination.strength);
    configuration.illumination = illumination;

    for (auto& elem : j["sceneObjects"]) {
        SceneObject object;
        elem["filePath"].get_to(object.filePath);
        elem["scale"].get_to(object.scale);
        elem["xDeslocation"].get_to(object.xDeslocation);

        configuration.sceneObjects.push_back(object);
    }
}

class ConfigReader
{
public:
	ConfigReader(string const& configFilePath)
	{
		std::ifstream ifs(configFilePath);
		json configurationFile = json::parse(ifs);

		std::cout << "______ config file : start ______" << std::endl;
		std::cout << configurationFile << std::endl;
		std::cout << "______ config file : end ________" << std::endl;

        Configuration configuration;
        from_json(configurationFile, configuration);
	}
};