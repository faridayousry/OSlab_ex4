
source backup_restore_lib.sh

dir1="$1"
dir2="$2"
key="$3"


	if [ $# -eq 0 ]; then 
		echo "Help msg: the correct way to use the program is as follows:
 ./restore.sh <name of dir containing backup> <name of dir to restore to> <decryption key>"
		exit 1
	fi



	if [ $# -ne 3 ]; then
		echo "Incorrect number of parameters"
		exit 1
	fi


	if [ ! -d "$dir1" ]; then 
		echo "Invalid backup directory to restore from" 
		exit 1		
	fi


	if [ ! -d "$dir2" ]; then
	 
		echo "Invalid restore directory to restore to"
		exit 1		
	fi




restore $dir1 $dir2 $key















