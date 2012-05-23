/* File : example.i */
%module geek_script_wrapper

%{
#include <script_interface_wrapper.hpp>
#include <geek/toolkits/component/component_wrapper.hpp>
%}


/* Let's just grab the original header file here */


%include <script_interface_wrapper.hpp>
%include <geek/toolkits/component/component_wrapper.hpp>