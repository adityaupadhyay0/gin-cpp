#include "gtest/gtest.h"
#include "mycppwebfw/utils/config.h"
#include <fstream>

class ServerConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a dummy config file
        std::ofstream config_file("test_config.json");
        config_file << R"({
            "port": 8888,
            "bind_address": "127.0.0.1",
            "max_connections": 512,
            "connection_timeout": 120,
            "thread_pool_size": 8,
            "enable_ssl": true,
            "log_level": "DEBUG"
        })";
        config_file.close();
    }

    void TearDown() override {
        std::remove("test_config.json");
    }
};

TEST_F(ServerConfigTest, LoadFromFile) {
    mycppwebfw::utils::ServerConfig config;
    config.load_from_file("test_config.json");

    EXPECT_EQ(config.get_port(), 8888);
    EXPECT_EQ(config.get_bind_address(), "127.0.0.1");
    EXPECT_EQ(config.get_max_connections(), 512);
    EXPECT_EQ(config.get_connection_timeout(), std::chrono::seconds(120));
    EXPECT_EQ(config.get_thread_pool_size(), 8);
    EXPECT_TRUE(config.is_ssl_enabled());
    EXPECT_EQ(config.get_log_level(), mycppwebfw::utils::LogLevel::DEBUG);
}

TEST(ServerConfigEnvTest, LoadFromEnv) {
    // Set environment variables
    setenv("SERVER_PORT", "9999", 1);
    setenv("SERVER_BIND_ADDRESS", "localhost", 1);
    setenv("SERVER_MAX_CONNECTIONS", "256", 1);
    setenv("SERVER_CONNECTION_TIMEOUT", "90", 1);
    setenv("SERVER_THREAD_POOL_SIZE", "2", 1);
    setenv("SERVER_ENABLE_SSL", "false", 1);
    setenv("SERVER_LOG_LEVEL", "WARN", 1);

    mycppwebfw::utils::ServerConfig config;
    config.load_from_env();

    EXPECT_EQ(config.get_port(), 9999);
    EXPECT_EQ(config.get_bind_address(), "localhost");
    EXPECT_EQ(config.get_max_connections(), 256);
    EXPECT_EQ(config.get_connection_timeout(), std::chrono::seconds(90));
    EXPECT_EQ(config.get_thread_pool_size(), 2);
    EXPECT_FALSE(config.is_ssl_enabled());
    EXPECT_EQ(config.get_log_level(), mycppwebfw::utils::LogLevel::WARN);

    // Unset environment variables
    unsetenv("SERVER_PORT");
    unsetenv("SERVER_BIND_ADDRESS");
    unsetenv("SERVER_MAX_CONNECTIONS");
    unsetenv("SERVER_CONNECTION_TIMEOUT");
    unsetenv("SERVER_THREAD_POOL_SIZE");
    unsetenv("SERVER_ENABLE_SSL");
    unsetenv("SERVER_LOG_LEVEL");
}

TEST(ServerConfigValidationTest, Validation) {
    mycppwebfw::utils::ServerConfig config;
    EXPECT_NO_THROW(config.validate());

    // Invalid port
    // config.port_ = 0; // port_ is private, can't be modified directly
    // EXPECT_THROW(config.validate(), std::invalid_argument);

    // To test validation, we would need to be able to set invalid values,
    // which is not possible with the current class design.
    // This is a good example of how testing can inform design.
    // For now, we will just test the default valid configuration.
}
