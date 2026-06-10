#include <gtest/gtest.h>

#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(IntegrationTest, HealthEndpointShouldReturnUp) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/health");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 200);

    const auto body = json::parse(res->body);
    EXPECT_EQ(body["status"], "UP");
}

TEST(IntegrationTest, SolveEndpointShouldReturnNonEmptyBody) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/solve");

    ASSERT_TRUE(res != nullptr);
    EXPECT_FALSE(res->body.empty());
}

TEST(IntegrationTest, HealthEndpointReturnsJson) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/health");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("UP"), std::string::npos);
}

TEST(IntegrationTest, UnknownEndpointReturns404) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/nonexistent");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 404);
}

TEST(IntegrationTest, SolveWithBadFileReturns500) {
    httplib::Client client("localhost", 8080);
    const auto res = client.Get("/solve?file=/nonexistent/bad.txt");

    ASSERT_TRUE(res != nullptr);
    EXPECT_EQ(res->status, 500);
    const auto body = json::parse(res->body);
    EXPECT_TRUE(body.contains("error"));
}
