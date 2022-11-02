/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ugdaniel <ugdaniel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 20:39:59 by ugdaniel          #+#    #+#             */
/*   Updated: 2022/11/01 18:16:49 by ugdaniel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <string>

# define HTTP_PROTOCOL						"HTTP/1.1"
# define DEFAULT_PORT						8080

# define CR									(u_char)'\r'
# define LF									(u_char)'\n'
# define CRLF								"\r\n"

// Methods
# define METHOD_UNKNOWN						0x1
# define METHOD_GET							0x2
# define METHOD_HEAD						0x4
# define METHOD_POST						0x8
# define METHOD_DELETE						0x10
# define ALL_METHODS						0x20

// Status codes
# define STATUS_OK							200
# define STATUS_CREATED						201
# define STATUS_ACCEPTED					202
# define STATUS_NO_CONTENT					204
# define STATUS_MOVED_PERMANENTLY			301
# define STATUS_FOUND						302
# define STATUS_SEE_OTHER					303
# define STATUS_BAD_REQUEST					400
# define STATUS_FORBIDDEN					403
# define STATUS_NOT_FOUND					404
# define STATUS_METHOD_NOT_ALLOWED			405
# define STATUS_PAYLOAD_TOO_LARGE			413
# define STATUS_INTERNAL_SERVER_ERROR		500
# define STATUS_NOT_IMPLEMENTED				501
# define STATUS_SERVICE_UNAVAILABLE			503
# define STATUS_HTTP_VERSION_NOT_SUPPORTED	505

// Header fields
# define HEADER_ACCEPT						"Accept"
# define HEADER_CONNECTION					"Connection"
# define HEADER_CONTENT_TYPE				"Content-type"
# define HEADER_CONTENT_LENGTH				"Content-length"
# define HEADER_DATE						"Date"
# define HEADER_HOST						"Host"
# define HEADER_LOCATION					"Location"
# define HEADER_SERVER						"Server"

// MIME types
# define MIME_BMP							"image/bmp"
# define MIME_CSS							"text/css"
# define MIME_GIF							"image/gif"
# define MIME_HTM							"text/htm"
# define MIME_HTML							"text/html"
# define MIME_ICO							"image/x-icon"
# define MIME_JPG							"image/jpg"
# define MIME_JPEG							"image/jpeg"
# define MIME_JS							"text/javascript"
# define MIME_PNG							"image/png"
# define MIME_PDF							"application/pdf"
# define MIME_SH							"application/x-sh"
# define MIME_PLAIN_TEXT					"text/plain"

# define EXTENSION_BMP						".bmp"
# define EXTENSION_CSS						".css"
# define EXTENSION_GIF						".gif"
# define EXTENSION_HTM						".htm"
# define EXTENSION_HTML						".html"
# define EXTENSION_ICO						".ico"
# define EXTENSION_JPG						".jpg"
# define EXTENSION_JPEG						".jpeg"
# define EXTENSION_JS						".js"
# define EXTENSION_PNG						".png"
# define EXTENSION_PDF						".pdf"
# define EXTENSION_SH						".sh"
# define EXTENSION_PLAIN_TEXT				""

typedef struct s_listen
{
	std::string	host;
	int			port;
}t_listen;

#endif // HTTP_HPP
