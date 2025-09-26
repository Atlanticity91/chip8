--- ENVIRONEMENT VARIABLES
visual_studio = os.getenv( 'VS_PATH' )

--- OUTPUT DIRS
OutputDirs = { }

OutputDirs[ 'Solution' ] = '%{wks.location}Solution/'
OutputDirs[ 'Bin' ] = '%{wks.location}bin/'
OutputDirs[ 'BinInt' ] = '%{wks.location}bin-int/'

--- EXTENSION LIBRARIES
ExtensionDirs = { }

--- INCLUDE DIRS
IncludeDirs = { }

--- EXAMPLES PROJECT
IncludeDirs[ 'chip8' ] = '%{wks.location}src/'
