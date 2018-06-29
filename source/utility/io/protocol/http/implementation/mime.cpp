#include "mime.h"

#include <algorithm>
#include <stdexcept>


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


std::string MIME::getByFileNameExtension(std::string const &extension) const {
    for (auto const &mime: m_mimes) {
        if (std::find(mime.extensions.begin(), mime.extensions.end(), extension) != mime.extensions.end())
            return mime.type;
    }
    throw std::runtime_error("unknown mime by extension '" + extension + "'"); // ----->
}


MIME::MIME() {    
    m_mimes.push_back( { { "abw" }          , "application/x-abiword" } );
    m_mimes.push_back( { { "arc" }          , "application/octet-stream" } );
    m_mimes.push_back( { { "azw" }          , "application/vnd.amazon.ebook" } );
    m_mimes.push_back( { { "bin" }          , "application/octet-stream" } );
    m_mimes.push_back( { { "bz"  }          , "application/x-bzip" } );
    m_mimes.push_back( { { "bz2" }          , "application/x-bzip2" } );
    m_mimes.push_back( { { "csh" }          , "application/x-csh" } );
    m_mimes.push_back( { { "doc" }          , "application/msword" } );
    m_mimes.push_back( { { "eot" }          , "application/vnd.ms-fontobject" } );
    m_mimes.push_back( { { "epub"}          , "application/epub+zip" } );
    m_mimes.push_back( { { "jar" }          , "application/java-archive" } );
    m_mimes.push_back( { { "js"  }          , "application/javascript" } );
    m_mimes.push_back( { { "json" }         , "application/json" } );
    m_mimes.push_back( { { "mpkg" }         , "application/vnd.apple.installer+xml" } );
    m_mimes.push_back( { { "odp" }          , "application/vnd.oasis.opendocument.presentation" } );
    m_mimes.push_back( { { "ods" }          , "application/vnd.oasis.opendocument.spreadsheet" } );
    m_mimes.push_back( { { "odt" }          , "application/vnd.oasis.opendocument.text" } );
    m_mimes.push_back( { { "ogx" }          , "application/ogg" } );
    m_mimes.push_back( { { "pdf" }          , "application/pdf" } );
    m_mimes.push_back( { { "ppt" }          , "application/vnd.ms-powerpoint" } );
    m_mimes.push_back( { { "rar" }          , "application/x-rar-compressed" } );
    m_mimes.push_back( { { "rtf" }          , "application/rtf" } );
    m_mimes.push_back( { { "sh"  }          , "application/x-sh" } );
    m_mimes.push_back( { { "swf" }          , "application/x-shockwave-flash" } );
    m_mimes.push_back( { { "tar" }          , "application/x-tar" } );
    m_mimes.push_back( { { "vsd" }          , "application/vnd.visio" } );
    m_mimes.push_back( { { "xhtml" }        , "application/xhtml+xml" } );
    m_mimes.push_back( { { "xls" }          , "application/vnd.ms-excel" } );
    m_mimes.push_back( { { "xml" }          , "application/xml" } );
    m_mimes.push_back( { { "xul" }          , "application/vnd.mozilla.xul+xml" } );
    m_mimes.push_back( { { "zip" }          , "application/zip" } );
    m_mimes.push_back( { { "7z"  }          , "application/x-7z-compressed" } );

    m_mimes.push_back( { { "aac" }          , "audio/aac" } );
    m_mimes.push_back( { { "mid", "midi" }  , "audio/midi" } );
    m_mimes.push_back( { { "oga" }          , "audio/ogg" } );
    m_mimes.push_back( { { "wav" }          , "audio/x-wav" } );
    m_mimes.push_back( { { "weba" }         , "audio/webm" } );
    m_mimes.push_back( { { "mp3" }          , "audio/mpeg" } );

    m_mimes.push_back( { { "avi" }          , "video/x-msvideo" } );
    m_mimes.push_back( { { "mpeg" }         , "video/mpeg" } );
    m_mimes.push_back( { { "ogv" }          , "video/ogg" } );
    m_mimes.push_back( { { "ts"  }          , "video/vnd.dlna.mpeg-tts" } );
    m_mimes.push_back( { { "webm" }         , "video/webm" } );
    m_mimes.push_back( { { "3gp" }          , "video/3gpp" } );
    m_mimes.push_back( { { "3g2" }          , "video/3gpp2" } );

    m_mimes.push_back( { { "css" }          , "text/css" } );
    m_mimes.push_back( { { "csv" }          , "text/csv" } );
    m_mimes.push_back( { { "htm", "html" }  , "text/html" } );
    m_mimes.push_back( { { "ics" }          , "text/calendar" } );

    m_mimes.push_back( { { "gif" }          , "image/gif" } );
    m_mimes.push_back( { { "ico" }          , "image/x-icon" } );
    m_mimes.push_back( { { "jpeg", "jpg" }  , "image/jpeg" } );
    m_mimes.push_back( { { "png" }          , "image/png" } );
    m_mimes.push_back( { { "svg" }          , "image/svg+xml" } );
    m_mimes.push_back( { { "tif", "tiff" }  , "image/tiff" } );
    m_mimes.push_back( { { "webp" }         , "image/webp" } );

    m_mimes.push_back( { { "otf" }          , "font/otf" } );
    m_mimes.push_back( { { "ttf" }          , "font/ttf" } );
    m_mimes.push_back( { { "woff" }         , "font/woff" } );
    m_mimes.push_back( { { "woff2" }        , "font/woff2" } );
}


} // implementation
} // http
} // protocol
} // io
} // utility
