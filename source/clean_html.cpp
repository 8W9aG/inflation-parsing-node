#include "clean_html.h"

#include <iostream>

#include <tidy.h>
#include <tidybuffio.h>

namespace inflation {
namespace node {

std::string cleanHTML(const std::string &html) {
    TidyDoc tdoc = tidyCreate();
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};
    if (tidyOptSetBool(tdoc, TidyXhtmlOut, yes) != yes) {
        std::cout << "Failed to set Tidy to XHTML output" << std::endl;
        tidyRelease(tdoc);
        return;
    }
    if (tidyOptSetBool(tdoc, TidyForceOutput, yes) != yes) {
        std::cout << "Failed to set Tidy to force output" << std::endl;
        tidyRelease(tdoc);
        return;
    }
    if (tidySetErrorBuffer(tdoc, &errbuf) < 0) {
        std::cout << "Failed to set error buffer in Tidy HTML parser" << std::endl;
        tidyRelease(tdoc);
        return;
    }
    if (tidyParseString(tdoc, html.c_str()) < 0) {
        std::cout << "Failed to parse the HTML string in Tidy" << std::endl;
        tidyBufFree(&errbuf);
        tidyRelease(tdoc);
        return;
    }
    if (tidyCleanAndRepair(tdoc) < 0) {
        std::cout << "Failed to clean and repair the HTML in Tidy" << std::endl;
        tidyBufFree(&errbuf);
        tidyRelease(tdoc);
        return;
    }
    if (tidySaveBuffer(tdoc, &output) < 0) {
        std::cout << "Failed to save buffer in Tidy" << std::endl;
        tidyBufFree(&errbuf);
        tidyRelease(tdoc);
        return;
    }
    std::string clean_html((const char *)output.bp, output.size);
    tidyBufFree(&output);
    tidyBufFree(&errbuf);
    tidyRelease(tdoc);
    return clean_html;
}

}  // namespace node
}  // namespace inflation
