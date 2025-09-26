#!/usr/bin/env python

# --- INCLUDES
import os
import platform
import shutil
import subprocess
import sys

# --- GLOBALS
FILE_EXT = [
	'.sln',
	'.vcxproj',
	'.filters',
	'.user',
]

FOLDERS = [
	'bin',
	'bin-int',
	'Solution',
	'.vs'
]

# --- FUNCTIONS
'''
	clear_paths method
	@note : Delete output folders and solution files to regenerated clean solution.
'''
def clear_paths( ) :
	print( '\n> Clearing output folders :' )

	for path in os.listdir( '.' ) :
		if path in FOLDERS :
			print( f'\033[33m> Deleted Folder : {path}\033[0m' )

			shutil.rmtree( path )
		elif os.path.splitext( path )[ -1 ].lower( ) in FILE_EXT :
			print( f'\033[33m> Deleted File : {path}\033[0m' )

			os.remove( path )

'''
	call_premake method
	@note : Call premake for solution file generation.
'''
def call_premake( ) :
	print( '\n> Calling Premake5 :')

	premake_call = [ '--file=Build\\Build.lua' ] + sys.argv[ 1: ]

	if platform.system( ) == 'Windows' :
		premake_call.insert( 0, 'Build\\Premake\\Windows\\premake5.exe' )
	elif platform.system( ) == 'Linux' :
		premake_call.insert( 0, 'Build\\Premake\\Linux\\premake5' )

	if subprocess.call( premake_call ) == 0 :
		print( f'\n\033[32m> Generation succeded.\033[0m' )
	else :
		print( f'\n\033[31m> Generation Failed.\033[0m' )

# --- MAIN LOGIC
if __name__ == "__main__" :
	print( '=== Setup Chip8 ===' )

	clear_paths( )
	call_premake( )
