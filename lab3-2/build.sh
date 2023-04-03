#!/bin/bash
gcc main.c -o main
rm bin/*
ln main bin/mk_dir
ln main bin/show_files
ln main bin/rm_dir
ln main bin/create_file
ln main bin/show_file_content
ln main bin/rm_file
ln main bin/create_sym_link
ln main bin/show_sym_link_content
ln main bin/show_sym_linked_file_content
ln main bin/rm_sym_link
ln main bin/create_hard_link
ln main bin/rm_hard_link
ln main bin/show_mod_and_nlink
ln main bin/change_access_permissions
