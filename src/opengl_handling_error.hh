#pragma once

void test_opengl_error(const char *function_name, int line);
#define TEST_OPENGL_ERROR() test_opengl_error( __FUNCTION__, __LINE__)
