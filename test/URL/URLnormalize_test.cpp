#include <gtest/gtest.h>
#include "url_handler.h"


TEST(URLNormalize, LowercasesSchemeAndHost)
{
    URL url("HTTP://Example.COM/Path");
    EXPECT_EQ(url.normalize_url("HTTP://Example.COM/Path"), "http://example.com/Path");
}
