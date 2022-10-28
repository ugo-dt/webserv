/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 14:23:54 by madaniel          #+#    #+#             */
/*   Updated: 2022/10/28 15:13:44 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Autoindex.hpp"

Autoindex::Autoindex(void)
	:_result("")
{
}

Autoindex::~Autoindex(void)
{
}

void
Autoindex::_create_link_and_sort(const t_listen& host_port, struct dirent *entity, std::string path)
{
	std::string		line;

	if (path == "/")
		path = "";
	line = "\t\t\t<p><a ";
	if (entity->d_type == DT_DIR)
	{
		line += "class=\"icon dir\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
			path + "/" + entity->d_name + "/" + "\">" + entity->d_name + "</a></p>\n";
	}
	else if (entity->d_type == DT_REG)
	{
		line += "class=\"icon file\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
			path + "/" + entity->d_name + "\">" + entity->d_name + "</a></p>\n";
	}
	else
	{
		line += "class=\"icon def\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
			path + "/" + entity->d_name + "\">" + entity->d_name + "</a></p>\n";
	}
	WS_VALUE_LOG("Path", line);
	_links.insert(std::make_pair(str_to_lower(entity->d_name), line));
}

/**
 * @brief Takes absolute path and gives index page. Empty path is set to '/'
 */
const std::string
Autoindex::get_index(std::string path, const t_listen& host_port)
{
	DIR				*dir;
	struct dirent	*entity;

	if (path == "")
		path = "/";
	if ((dir = opendir(path.c_str())) == NULL)
		return (_result);
	_result =
	"<!DOCTYPE html>\n"
	"<html>\n"
		"\t<meta name=\"color-scheme\" content=\"light dark\">\n"
		"\t<style>\n"
		"\t\th1\n"
		"\t\t{\n"
			"\t\t\tborder-bottom: 1px solid #c0c0c0;\n"
			"\t\t\tmargin-bottom: 10px;\n"
			"\t\t\tpadding-bottom: 10px;\n"
			"\t\t\twhite-space: nowrap;\n"
		"\t\t}\n"
		"\t\ta.icon\n"
		"\t\t{\n"
			"\t\t\tpadding-inline-start: 1.5em;\n"
			"\t\t\ttext-decoration: none;\n"
			"\t\t\tuser-select: auto;\n"
		"\t\t}\n"
		"\t\ta.icon:hover\n" 
		"\t\t\t{\n"
			"\t\t\ttext-decoration: underline;\n"
		"\t\t}\n"
		"\t\ta.file\n"
		"\t\t{\n"
    		"\t\t\tbackground : url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAAABnRSTlMAAAAAAABupgeRAAABEElEQVR42nRRx3HDMBC846AHZ7sP54BmWAyrsP588qnwlhqw/k4v5ZwWxM1hzmGRgV1cYqrRarXoH2w2m6qqiqKIR6cPtzc3xMSML2Te7XZZlnW7Pe/91/dX47WRBHuA9oyGmRknzGDjab1ePzw8bLfb6WRalmW4ip9FDVpYSWZgOp12Oh3nXJ7nxoJSGEciteP9y+fH52q1euv38WosqA6T2gGOT44vry7BEQtJkMAMMpa6JagAMcUfWYa4hkkzAc7fFlSjwqCoOUYAF5RjHZPVCFBOtSBGfgUDji3c3jpibeEMQhIMh8NwshqyRsBJgvF4jMs/YlVR5KhgNpuBLzk0OcUiR3CMhcPaOzsZiAAA/AjmaB3WZIkAAAAASUVORK5CYII=\") left top no-repeat;\n"
  		"\t\t}\n"
		"\t\ta.dir\n"
		"\t\t{\n"
    		"\t\t\tbackground : url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABt0lEQVR42oxStZoWQRCs2cXdHTLcHZ6EjAwnQWIkJyQlRt4Cd3d3d1n5d7q7ju1zv/q+mh6taQsk8fn29kPDRo87SDMQcNAUJgIQkBjdAoRKdXjm2mOH0AqS+PlkP8sfp0h93iu/PDji9s2FzSSJVg5ykZqWgfGRr9rAAAQiDFoB1OfyESZEB7iAI0lHwLREQBcQQKqo8p+gNUCguwCNAAUQAcFOb0NNGjT+BbUC2YsHZpWLhC6/m0chqIoM1LKbQIIBwlTQE1xAo9QDGDPYf6rkTpPc92gCUYVJAZjhyZltJ95f3zuvLYRGWWCUNkDL2333McBh4kaLlxg+aTmyL7c2xTjkN4Bt7oE3DBP/3SRz65R/bkmBRPGzcRNHYuzMjaj+fdnaFoJUEdTSXfaHbe7XNnMPyqryPcmfY+zURaAB7SHk9cXSH4fQ5rojgCAVIuqCNWgRhLYLhJB4k3iZfIPtnQiCpjAzeBIRXMA6emAqoEbQSoDdGxFUrxS1AYcpaNbBgyQBGJEOnYOeENKR/iAd1npusI4C75/c3539+nbUjOgZV5CkAU27df40lH+agUdIuA/EAgDmZnwZlhDc0wAAAABJRU5ErkJggg==\") left top no-repeat;\n"
  		"\t\t}\n"
		"\t\ta.def\n"
		"\t\t{\n"
    		"\t\t\tbackground : url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABt0lEQVR42oxStZoWQRCs2cXdHTLcHZ6EjAwnQWIkJyQlRt4Cd3d3d1n5d7q7ju1zv/q+mh6taQsk8fn29kPDRo87SDMQcNAUJgIQkBjdAoRKdXjm2mOH0AqS+PlkP8sfp0h93iu/PDji9s2FzSSJVg5ykZqWgfGRr9rAAAQiDFoB1OfyESZEB7iAI0lHwLREQBcQQKqo8p+gNUCguwCNAAUQAcFOb0NNGjT+BbUC2YsHZpWLhC6/m0chqIoM1LKbQIIBwlTQE1xAo9QDGDPYf6rkTpPc92gCUYVJAZjhyZltJ95f3zuvLYRGWWCUNkDL2333McBh4kaLlxg+aTmyL7c2xTjkN4Bt7oE3DBP/3SRz65R/bkmBRPGzcRNHYuzMjaj+fdnaFoJUEdTSXfaHbe7XNnMPyqryPcmfY+zURaAB7SHk9cXSH4fQ5rojgCAVIuqCNWgRhLYLhJB4k3iZfIPtnQiCpjAzeBIRXMA6emAqoEbQSoDdGxFUrxS1AYcpaNbBgyQBGJEOnYOeENKR/iAd1npusI4C75/c3539+nbUjOgZV5CkAU27df40lH+agUdIuA/EAgDmZnwZlhDc0wAAAABJRU5ErkJggg==\") left top no-repeat;\n"
  		"\t\t}\n"
		"\t</style>\n"
		"\t<head>\n"
			"\t\t<title id=\"title\"> Index of " + path + "</title>\n"
		"\t</head>\n"
		"\t<body>\n"
			"\t\t<h1>Index of " + path + "</h1>\n";

	for (entity = readdir(dir); entity; entity = readdir(dir))
	{
		if ((strcmp(entity->d_name, ".") != 0))// && (strcmp(entity->d_name, "..") != 0)
			_create_link_and_sort(host_port, entity, path);
		WS_VALUE_LOG("Type", entity->d_type); // reg 4, dir 8
		WS_VALUE_LOG("Name", entity->d_name);
	}
	for (std::map<std::string,std::string>::iterator it = _links.begin(); it != _links.end() ; it++)
	{
		_result += it->second;
	}

	_result +=
		"\t</body>\n"
	"</html>";
	closedir(dir);
	return (_result);
}
