#include <gtest/gtest.h>
#include "url_handler.h"

TEST(URLHandler, ResolveAbsoluteURL)
{
    URL base("https://example.com/docs/index.html");

    EXPECT_EQ(
        base.resolve_url("https://google.com/page"),
        "https://google.com/page"
    );
}

TEST(URLHandler, ResolveProtocolRelative)
{
    URL base("https://example.com/docs/index.html");

    EXPECT_EQ(
        base.resolve_url("//cdn.example.com/logo.png"),
        "https://cdn.example.com/logo.png"
    );
}

TEST(URLHandler, ResolveRootRelative)
{
    URL base("https://example.com/docs/index.html");

    EXPECT_EQ(
        base.resolve_url("/about"),
        "https://example.com/about"
    );
}

TEST(URLHandler, ResolveRelative)
{
    URL base("https://example.com/docs/index.html");

    EXPECT_EQ(
        base.resolve_url("contact.html"),
        "https://example.com/docs/contact.html"
    );
}

TEST(URLHandler, ResolveRelativeWithPort)
{
    URL base("https://example.com:8080/docs/index.html");

    EXPECT_EQ(
        base.resolve_url("contact.html"),
        "https://example.com:8080/docs/contact.html"
    );
}

TEST(URLHandler, ResolveExtra)
{
    URL base("https://example.com/docs/tutorial/index.html");
EXPECT_EQ(
    base.resolve_url("../about.html"),
    "https://example.com/docs/tutorial/../about.html"
);
}