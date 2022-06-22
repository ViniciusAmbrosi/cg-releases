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
    float lightColorR;
    float lightColorG;
    float lightColorB;

    float lightPositionX;
    float lightPositionY;
    float lightPositionZ;
    
    float ka;
    float ks;
    float kd;
    float n;
};

struct SceneObject
{
    std::string filePath;
    float scale;
    float xDeslocation;
    float zDeslocation;
    float rotation;
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
    j.at("illumination").at("lightColor").at("r").get_to(illumination.lightColorR);
    j.at("illumination").at("lightColor").at("g").get_to(illumination.lightColorG);
    j.at("illumination").at("lightColor").at("b").get_to(illumination.lightColorB);

    j.at("illumination").at("lightPosition").at("x").get_to(illumination.lightPositionX);
    j.at("illumination").at("lightPosition").at("y").get_to(illumination.lightPositionY);
    j.at("illumination").at("lightPosition").at("z").get_to(illumination.lightPositionZ);

    j.at("illumination").at("ka").get_to(illumination.ka);
    j.at("illumination").at("kd").get_to(illumination.kd);
    j.at("illumination").at("ks").get_to(illumination.ks);
    j.at("illumination").at("n").get_to(illumination.n);

    configuration.illumination = illumination;

    for (auto& elem : j["sceneObjects"]) {
        SceneObject object;
        elem["filePath"].get_to(object.filePath);
        elem["scale"].get_to(object.scale);
        elem["xDeslocation"].get_to(object.xDeslocation);
        elem["zDeslocation"].get_to(object.zDeslocation);
        elem["rotation"].get_to(object.rotation);

        configuration.sceneObjects.push_back(object);
    }
}

class ConfigReader
{
public:
    Configuration configuration;

	ConfigReader(string const& configFilePath)
	{
		std::ifstream ifs(configFilePath);
		json configurationFile = json::parse(ifs);

		std::cout << "______ config file : start ______" << std::endl;
		std::cout << configurationFile << std::endl;
		std::cout << "______ config file : end ________" << std::endl;

        from_json(configurationFile, configuration);
	}
};