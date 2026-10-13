import std;
import std.compat;
#include "test.hpp"
#include "allocator.hpp"
#include <cassert>


#ifdef _WIN32_WCE
#undef DebugBreak
#pragma warning(disable                                                        \
                : 4201) // nonstandard extension used: nameless struct/union
#include <windows.h>
#endif

test_runner *test_runner::_tests = 0;
size_t test_runner::_memory_fail_threshold = 0;
bool test_runner::_memory_fail_triggered = false;
jmp_buf test_runner::_failure_buffer;
const char *test_runner::_failure_message;
const char *test_runner::_temp_path;

static size_t g_memory_total_size = 0;
static size_t g_memory_total_count = 0;
static size_t g_memory_fail_triggered = false;

static void *custom_allocate(size_t size)
{
  if (test_runner::_memory_fail_threshold > 0 &&
      test_runner::_memory_fail_threshold < g_memory_total_size + size)
  {
    g_memory_fail_triggered = true;
    test_runner::_memory_fail_triggered = true;

    return 0;
  }
  else
  {
    void *ptr = memory_allocate(size);
    if (!ptr)
      return 0;

    g_memory_total_size += memory_size(ptr);
    g_memory_total_count++;

    return ptr;
  }
}

static void *custom_allocate_throw(size_t size)
{
  void *result = custom_allocate(size);

  if (!result)
    throw std::bad_alloc();

  return result;
}

static void custom_deallocate(void *ptr)
{
  assert(ptr);

  g_memory_total_size -= memory_size(ptr);
  g_memory_total_count--;

  memory_deallocate(ptr);
}

static void replace_memory_management()
{
  std::println("replace_memory_management");
  // create some document to touch original functions
  {
    pugi::XmlDocument doc;
    std::println("replace_memory_management");
    auto node = doc.append_child();
    node.set_name("node");
    std::println("replace_memory_management");
  }
  std::println("replace_memory_management");
  // replace functions
  pugi::set_memory_management_functions(custom_allocate, custom_deallocate);
  std::println("replace_memory_management");
}

static bool run_test(test_runner *test, const char *test_name,
                     pugi::allocation_function allocate)
{
#ifndef PUGIXML_NO_EXCEPTIONS
  try
  {
#endif
    g_memory_total_size = 0;
    g_memory_total_count = 0;
    g_memory_fail_triggered = false;
    test_runner::_memory_fail_threshold = 0;
    test_runner::_memory_fail_triggered = false;

    pugi::set_memory_management_functions(allocate, custom_deallocate);

    volatile int result = setjmp(test_runner::_failure_buffer);

    if (result)
    {
      printf("Test %s failed: %s\n", test_name, test_runner::_failure_message);
      return false;
    }

    test->run();

    if (test_runner::_memory_fail_triggered)
    {
      printf("Test %s failed: unguarded memory fail triggered\n", test_name);
      return false;
    }

    if (g_memory_total_size != 0 || g_memory_total_count != 0)
    {
      printf(
          "Test %s failed: memory leaks found (%u bytes in %u allocations)\n",
          test_name, static_cast< unsigned int >(g_memory_total_size),
          static_cast< unsigned int >(g_memory_total_count));
      return false;
    }

    return true;
#ifndef PUGIXML_NO_EXCEPTIONS
  }
  catch (const std::exception &e)
  {
    printf("Test %s failed: exception %s\n", test_name, e.what());
    return false;
  }
  catch (...)
  {
    printf("Test %s failed for unknown reason\n", test_name);
    return false;
  }
#endif
}

int main(int, char **argv)
{
  // setup temp path as the executable folder
  std::println("Run pugixml test");
  std::string temp = argv[0];
  std::string::size_type slash = temp.find_last_of("\\/");
  temp.erase((slash != std::string::npos) ? slash + 1 : 0);

  test_runner::_temp_path = temp.c_str();

  replace_memory_management();

  unsigned int total = 0;
  unsigned int passed = 0;

  test_runner *test = 0; // gcc3 "variable might be used uninitialized in this
                         // function" bug workaround

  for (test = test_runner::_tests; test; test = test->_next)
  {
    std::println("Run test {}", test->_name);
    total++;
    passed += run_test(test, test->_name, custom_allocate);

    if (g_memory_fail_triggered)
    {
      // run tests that trigger memory failures twice - with an allocator that
      // returns NULL and with an allocator that throws
#ifndef PUGIXML_NO_EXCEPTIONS
      total++;
      passed += run_test(test, (test->_name + std::string(" (throw)")).c_str(),
                         custom_allocate_throw);
#endif
    }
  }

  unsigned int failed = total - passed;

  if (failed != 0)
    printf("FAILURE: %u out of %u tests failed.\n", failed, total);
  else
    printf("Success: %u tests passed.\n", total);

  return failed;
}

#ifdef _WIN32_WCE
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  return main(0, NULL);
}
#endif
