#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(){
	int shm_id;
	struct shmid_ds buffer;

	shm_id = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0666);
	shmctl(shm_id, IPC_STAT, &buffer);

	printf("Segment ID: %d\n", shm_id);
 	printf("Key: %d\n", (int)buffer.shm_perm.__key);
 	printf("Owner UID: %d\n", (int)buffer.shm_perm.uid);
 	printf("Mode: %o\n", (int)buffer.shm_perm.mode);
	printf("Size: %d\n", (int)buffer.shm_segsz);
 	printf("Number of attaches: %d\n", (int)buffer.shm_nattch);	

	shmctl(shm_id, IPC_RMID, &buffer);
	return 0;
}

/*	int shmget(key_t key, size_t size, int shmflag);

		key:
      			IPC_PRIVATE: Create a new Share-Memory segment.

		size:
      			The size of the Share-Memory 
			(The smallest unit is 4kb = 4096)

		shmflag:
      			S_IRUSR: Read memory segment
			S_IWUSR: Write memory segment
			IPC_CREAT :Make sure the Memory is new, not the Memory currently existing

		return:
			 Share-Memory ID

	int shmctl(int shm_id, int command, struct shmid_ds *buffer);

		shm_id:
			Share-Memory ID

		cmd:
			IPC_STAT: Assign "shmid_ds" from Share-Memory to Buffer
          		IPC_SET: Copy from Buffer to Share-Memory"
          		IPC_RMID: Remove Share-Memory

		buffer:
			Buffer for "shmid_ds"

	 struct shmid_ds {
                struct ipc_perm shm_perm;	operation perms 
                int     shm_segsz;              size of segment (bytes)
                time_t  shm_atime;              last attach time 
                time_t  shm_dtime;              last detach time
                time_t  shm_ctime;              last change time
                unsigned short  shm_cpid;       pid of creator
                unsigned short  shm_lpid;       pid of last operator 
                short   shm_nattch;             no. of current attaches

                -------------------the following are private----------------------

                unsigned short   shm_npages;    	size of segment (pages)
                unsigned long   *shm_pages;  		array of ptrs to frames -> SHMMAX
                struct vm_area_struct *attaches;	descriptors for attaches
        };
*/
