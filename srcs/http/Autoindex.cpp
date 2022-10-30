/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 14:23:54 by madaniel          #+#    #+#             */
/*   Updated: 2022/10/30 12:37:31 by ugdaniel         ###   ########.fr       */
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
		if (strcmp(entity->d_name, "..") == 0)
		{
			line += "class=\"icon up\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
				"/" + path + entity->d_name + "/" + "\">" + "[parent directory]" + "</a></p>\n";
		}
		else
			line += "class=\"icon dir\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
				"/" + path + entity->d_name + "/" + "\">" + entity->d_name + "</a></p>\n";
	}
	else if (entity->d_type == DT_REG)
	{
		line += "class=\"icon file\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
			"/" + path + entity->d_name + "\">" + entity->d_name + "</a></p>\n";
	}
	else
	{
		line += "class=\"icon def\" draggable=\"true\" href=\"http://" + host_port.host + ":" + to_string(host_port.port) +
			"/" + path + entity->d_name + "\">" + entity->d_name + "</a></p>\n";
	}
	// WS_VALUE_LOG("link", line);
	_links.insert(std::pair<std::string, std::string>(str_to_lower(entity->d_name), line));
}

/**
 * @brief Takes absolute path and gives index page. Empty path is set to '/'
 */
const std::string
Autoindex::get_index(std::string path, const t_listen& host_port)
{
	DIR									*dir;
	struct dirent						*entity;
	std::string::reverse_iterator		rit = path.rbegin();
	std::string							tmp;

	if (!path.length())
		path = "/";
	else if (path != "./")
		while (path.compare(0, 2, "./") == 0)
			path.erase(0, 2);
	if ((dir = opendir(path.c_str())) == NULL)
		return (_result);
	if (*rit != '/')
		path += "/";
	
	// path displayed should be absolute from the server pov (e.g. "./" should be "/")
	if (path == "./")
		tmp = "/";
	else
		tmp = path;
	if (tmp[0] != '/')
		tmp.insert(0, 1, '/');
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
		"\t\ta.up\n"
		"\t\t{\n"
    		"\t\t\tbackground : url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAACM0lEQVR42myTA+w1RxRHz+zftmrbdlTbtq04qRGrCmvbDWp9tq3a7tPcub8mj9XZ3eHOGQdJAHw77/LbZuvnWy+c/CIAd+91CMf3bo+bgcBiBAGIZKXb19/zodsAkFT+3px+ssYfyHTQW5tr05dCOf3xN49KaVX9+2zy1dX4XMk+5JflN5MBPL30oVsvnvEyp+18Nt3ZAErQMSFOfelCFvw0HcUloDayljZkX+MmamTAMTe+d+ltZ+1wEaRAX/MAnkJdcujzZyErIiVSzCEvIiq4O83AG7LAkwsfIgAnbncag82jfPPdd9RQyhPkpNJvKJWQBKlYFmQA315n4YPNjwMAZYy0TgAweedLmLzTJSTLIxkWDaVCVfAbbiKjytgmm+EGpMBYW0WwwbZ7lL8anox/UxekaOW544HO0ANAshxuORT/RG5YSrjlwZ3lM955tlQqbtVMlWIhjwzkAVFB8Q9EAAA3AFJ+DR3DO/Pnd3NPi7H117rAzWjpEs8vfIqsGZpaweOfEAAFJKuM0v6kf2iC5pZ9+fmLSZfWBVaKfLLNOXj6lYY0V2lfyVCIsVzmcRV9Y0fx02eTaEwhl2PDrXcjFdYRAohQmS8QEFLCLKGYA0AeEakhCCFDXqxsE0AQACgAQp5w96o0lAXuNASeDKWIvADiHwigfBINpWKtAXJvCEKWgSJNbRvxf4SmrnKDpvZavePu1K/zu/due1X/6Nj90MBd/J2Cic7WjBp/jUdIuA8AUtd65M+PzXIAAAAASUVORK5CYII=\") left top no-repeat;\n"
  		"\t\t}\n"
		"\t</style>\n"
		"\t<head>\n"
			"\t\t<title id=\"title\"> Index of " + tmp + "</title>\n"
		"\t</head>\n"
		"\t<body>\n"
			"\t\t<h1>Index of " + tmp + "</h1>\n";

	for (entity = readdir(dir); entity; entity = readdir(dir))
	{
		if ((strcmp(entity->d_name, ".") != 0))// && (strcmp(entity->d_name, "..") != 0)
			_create_link_and_sort(host_port, entity, path);
		// if (entity->d_type == DT_DIR)
		// 	std::cout << "dir\t";
		// else if (entity->d_type == DT_REG)
		// 	std::cout << "file\t";
		// printf("%s\n", entity->d_name);
	}
	for (std::map<std::string,std::string>::iterator it = _links.begin(); it != _links.end() ; it++)
	{
		_result +=	it->second;
	}

	_result +=
		"\t</body>\n"
	"</html>\n";
	closedir(dir);
	return (_result);
}
