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
      [ "Parsing Example (JSON)", "index.html#subsec_parsing_example", null ],
      [ "Serialization Example (JSON)", "index.html#subsec_serializing_example", null ],
      [ "Typed Serialization via Schema", "index.html#subsec_serialization_macros", [
        [ "Schema Definition", "index.html#subsubsec_schema_definition", null ],
        [ "Schema Usage", "index.html#subsubsec_schema_usage", null ],
        [ "Copy and Move Semantics", "index.html#subsubsec_schema_semantics", null ],
        [ "Text Nodes", "index.html#subsubsec_schema_text", null ],
        [ "Type Name Uniqueness", "index.html#subsubsec_schema_unique_names", null ]
      ] ]
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
    [ "Benchmark", "index.html#sec_benchmark", null ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
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
"class_u_r_i.html#a254db5754960732769383810fc9bd1be",
"classiridium_1_1io_1_1_i_pipe_manager.html#acb974fb7adf77c72f677b3f28de6dfa9",
"classiridium_1_1io_1_1implementation_1_1_c_multiplexer_base.html#a5feeb27b61d3c8d7713ba42d1afddf26",
"classiridium_1_1parsing_1_1_schema_node_3_01_t_schema_type_1_1_a_r_r_a_y_00_01_t_item_01_4.html#a3cb66b2730ffdbf898adf01080b3bfcb",
"classiridium_1_1testing_1_1_i_unit_test_case.html",
"classiridium_1_1threading_1_1_synchronized_container.html#a347ec71e46eda22c5ce4c4c60ebec678",
"dir_4c131efd94f5fee91d7fbba85dd0d8a9.html",
"index.html#subsubsec_testing_cli",
"namespaceiridium_1_1testing.html",
"structiridium_1_1convertion_1_1implementation_1_1_t_convert_3_01int32__t_00_01std_1_1string_01_4.html",
"structiridium_1_1parsing_1_1_t_constraint_3_01_t_schema_type_1_1_a_r_r_a_y_00_01_t_item_00_01stdedf88f40b5953fc4084ea0f801679fc3.html#a1d5aaed1e84aba6f392827e148d846fc"
];

const SYNCONMSG = 'click to disable panel synchronization';
const SYNCOFFMSG = 'click to enable panel synchronization';
const LISTOFALLMEMBERS = 'List of all members';