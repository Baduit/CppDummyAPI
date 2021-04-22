#include <cctype>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <optional>

#include <httplib.h>
#include <nlohmann/json.hpp>

using nlohmann::json;

struct Element
{
	int id = 0;
	std::string message;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Element, id, message)

std::optional<int> parse_int(const std::string& str)
{
	auto id_digit = [](char c) { return std::isdigit(c); };
	auto it = std::find_if_not(str.cbegin(), str.cend(), id_digit);
	if (it == str.cend())
		return atoi(str.c_str());
	else
		return {};
}

int main()
{
	std::vector<Element> data;

	httplib::Server server;
	server.Get("/data",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			if (req.has_param("id"))
			{
				std::string id_param = req.get_param_value("id");
				auto id = parse_int(id_param);
				if (id)
				{
					auto it = std::find_if(data.begin(), data.end(), [id](const auto& e) { return e.id == *id; });
					if (it != data.end())
					{
						res.set_content(json(*it).dump(4), "application/json");
					}
					else
					{
						res.set_content("Element with the id " + id_param + " does not exist.", "text/plain");
						res.status = 400;
					}
				}
				else
				{
					res.set_content(id_param + " is not a valid id, please use a number", "text/plain");
					res.status = 400;
				}
			}
			else
			{
				res.set_content(json(data).dump(4), "application/json");
			}
		});
	server.Post("/data",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			try
			{
				auto element = json::parse(req.body).get<Element>();
				auto it = std::find_if(data.begin(), data.end(), [element](const auto& e) { return e.id == element.id; });
				if (it != data.end())
				{
					it->message = std::move(element.message);
					res.set_content("Element succeessfully updated", "text/plain");
				}
				else
				{
					data.push_back(std::move(element));
					res.set_content("Element succeessfully added.", "text/plain");
				}
			}
			catch (const nlohmann::json::exception& e)
			{
				using namespace std::string_literals;
				res.set_content("Error while parsing json: "s + e.what(), "text/plain");
				res.status = 400;
			}
		});
	server.Delete(R"(/data/(\d+))",
		[&](const httplib::Request& req, httplib::Response& res)
		{
			int id = std::atoi(req.matches[1].str().data());
			auto it = std::find_if(data.begin(), data.end(), [id](const auto& e) { return e.id == id; });
			if (it != data.end())
			{
				data.erase(it);
				res.set_content("Element succeessfully erased", "text/plain");
			}
			else
			{
				res.set_content("Element does not exist.", "text/plain");
				res.status = 404;
			}	
		});

	server.set_error_handler(
		[](const auto&, auto& res)
		{
			if (res.body.empty())
				res.set_content("<p>Error Status: <span style='color:red;'>" + std::to_string(res.status) + "</span></p>", "text/html");
			else
				res.set_content("<p>Error Status: <span style='color:red;'>" + std::to_string(res.status) + "</span></br> Error message: " + res.body + " </p>", "text/html");
		});

	server.set_exception_handler(
		[](const httplib::Request&, httplib::Response& res, std::exception& e)
		{
			using namespace std::string_literals;
			res.set_content("<h1>Error 500</h1><p>"s + e.what() + "</p>", "text/html");
			res.status = 500;
		});

	server.listen("0.0.0.0", 6579);
}