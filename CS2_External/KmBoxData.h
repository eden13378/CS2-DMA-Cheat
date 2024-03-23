#pragma once

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <string>
#include <fstream>

using namespace rapidjson;

class KmBoxDataGetter {
public:
	std::string type;

	std::string ip;

	int port;

	std::string uuid;

	KmBoxDataGetter(std::string path) {
		std::ifstream kmIfstream(path);
		IStreamWrapper kmJsonisw(kmIfstream);
		Document kmboxdata;
		kmboxdata.ParseStream<0>(kmJsonisw);

		type = kmboxdata["type"].GetString();

		if (type != "b") {
			ip = kmboxdata["ip"].GetString();

			port = kmboxdata["port"].GetInt64();

			uuid = kmboxdata["uuid"].GetString();
		}
	}
};
