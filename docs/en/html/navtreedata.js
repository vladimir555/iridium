/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Iridium", "index.html", [
    [ "Parsers and Serializers", "index.html#sec_parsers_serializers", [
      [ "The IParser Interface", "index.html#subsec_iparser", null ],
      [ "Obtaining a Parser", "index.html#subsec_obtaining_parser", null ],
      [ "Parsing Example (JSON)", "index.html#subsec_parsing_example", null ],
      [ "Serialization Example (JSON)", "index.html#subsec_serializing_example", null ],
      [ "Typed Serialization with NodeView", "index.html#subsec_serialization_macros", null ]
    ] ],
    [ "Logger Usage and Configuration", "index.html#sec_logging", [
      [ "Basic Logging", "index.html#subsec_logging_macros", null ],
      [ "Logger Configuration", "index.html#subsec_logger_configuration", null ],
      [ "Programmatic Configuration Example", "index.html#subsec_logger_config_example", null ],
      [ "Default Configuration", "index.html#subsec_default_config", null ]
    ] ],
    [ "Integrating Custom Types with <span class=\"tt\">iridium::convertion::convert</span>", "index.html#sec_custom_type_conversion", [
      [ "Example: Custom <span class=\"tt\">Point</span> Type", "index.html#subsec_custom_convert_example_fn", null ],
      [ "Impact of Custom <span class=\"tt\">convert</span> Specializations", "index.html#subsec_custom_convert_fn_impact", null ]
    ] ],
    [ "Handling Enumerations with DEFINE_ENUM", "index.html#sec_define_enum", [
      [ "Example Usage", "index.html#subsec_define_enum_example", null ],
      [ "Impact of <span class=\"tt\">DEFINE_ENUM</span>", "index.html#subsec_define_enum_impact", null ]
    ] ],
    [ "Multithreading Primitives", "index.html#sec_threading", [
      [ "Using CThread for Direct Thread Management", "index.html#subsec_cthread", null ],
      [ "Using Synchronized for Data Protection", "index.html#subsec_synchronized", [
        [ "Waiting on Conditions", "index.html#subsubsec_synchronized_wait", null ],
        [ "Interrupting Waits", "index.html#subsubsec_synchronized_interrupt", null ],
        [ "Example of Wait, Notify, and Interrupt", "index.html#subsubsec_synchronized_wait_example", null ]
      ] ],
      [ "Using CWorker for Task Processing", "index.html#subsec_cworker", null ],
      [ "Using CWorkerPool for Parallel Task Processing", "index.html#subsec_cworkerpool", null ]
    ] ],
    [ "Testing with Iridium", "index.html#sec_testing", [
      [ "Core Concepts", "index.html#subsec_testing_core_concepts", null ],
      [ "Writing Tests", "index.html#subsec_testing_writing_tests", [
        [ "The TEST Macro", "index.html#subsubsec_testing_test_macro", null ],
        [ "The ASSERT Macro", "index.html#subsubsec_testing_assertions", null ]
      ] ],
      [ "Running Tests", "index.html#subsec_testing_running_tests", [
        [ "The IMPLEMENT_TEST_MAIN Macro", "index.html#subsubsec_testing_main", null ],
        [ "Role of the Tester Class", "index.html#subsubsec_testing_tester", null ],
        [ "The ITestRunner Interface (for advanced scenarios)", "index.html#subsubsec_testing_itestrunner", null ],
        [ "Command-Line Arguments", "index.html#subsubsec_testing_cli", null ]
      ] ],
      [ "Mocking Dependencies", "index.html#subsec_testing_mocking", [
        [ "Defining Mock Classes", "index.html#subsubsec_testing_defining_mocks", null ],
        [ "Defining Mock Behavior", "index.html#subsubsec_testing_mock_behavior", null ],
        [ "Automatic Mock Object Creation and Provisioning via <span class=\"tt\">create()</span>", "index.html#subsubsec_testing_mock_auto_creation", null ]
      ] ],
      [ "Testing Call Sequences", "index.html#subsec_testing_sequences", [
        [ "Defining a Sequence (DEFINE_MOCK_SEQUENCE)", "index.html#subsubsec_testing_defining_sequences", null ],
        [ "Expectations in a Sequence (DEFINE_MOCK_SEQUENCE_EXPECTATION)", "index.html#subsubsec_testing_sequence_expectations", null ]
      ] ]
    ] ],
    [ "Versioning", "index.html#sec_versioning", null ],
    [ "Integrated Build System (CMake &amp; Conan)", "index.html#sec_build_system", [
      [ "Required Project Structure", "index.html#subsec_project_structure", null ],
      [ "CMake Integration", "index.html#subsec_cmake_integration", null ],
      [ "Conan Integration", "index.html#subsec_conan_integration", null ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Related Symbols", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Язык | Language", "user", null ],
    [ "English", "../../en/html/index.html", null ],
    [ "Русский", "../../ru/html/index.html", null ]
  ] ]
];

var NAVTREEINDEX =
[
"../../en/html/index.html",
"classiridium_1_1db_1_1implementation_1_1_c_connector.html#a91f91fae6c84560717d66364338ff158",
"classiridium_1_1io_1_1implementation_1_1_c_context.html#a3b110bb719f76b6f0d94331c0fcb28fd",
"classiridium_1_1parsing_1_1_i_node_type.html#acae1cce83114e749e94307c59be94c32",
"classiridium_1_1pattern_1_1_non_copyable.html#ae3c2afe37346ea92307157d57b73773e",
"classiridium_1_1threading_1_1_i_async_queue_pusher.html",
"classiridium_1_1threading_1_1implementation_1_1_c_worker_pusher_runnable.html#afefc2bd5f9d45e14de7ea46932e44bba",
"implementation_2file__stream_8h.html",
"namespaceiridium_1_1convertion_1_1implementation.html#af0d2742da5c43672188082f00c7c3b96",
"serialization_2node_8h.html#aac8f5e72dee27b1c9479d82a20f96f62",
"structiridium_1_1logging_1_1_log_stream.html#a5f0eaa6aa4281fd20d5c3eec4c61e0be"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';