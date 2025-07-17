#ifndef UNISTD_H
#define UNISTD_H


#define __NR_enosys   0
#define __NR_exit   1
#define __NR_fork   2
#define __NR_read   3
#define __NR_write   4
#define __NR_open   5
#define __NR_close   6
#define __NR_wait4   7
#define __NR_link   9
#define __NR_unlink   10
#define __NR_chdir   12
#define __NR_fchdir   13
#define __NR_mknod   14
#define __NR_chmod   15
#define __NR_chown   16
#define __NR_getfsstat   18
#define __NR_getpid   20
#define __NR_setuid   23
#define __NR_getuid   24
#define __NR_geteuid   25
#define __NR_ptrace   26
#if SOCKETS
#define __NR_recvmsg   27
#define __NR_sendmsg   28
#define __NR_recvfrom   29
#define __NR_accept   30
#define __NR_getpeername   31
#define __NR_getsockname   32
#endif /* SOCKETS */
#define __NR_access   33
#define __NR_chflags   34
#define __NR_fchflags   35
#define __NR_sync   36
#define __NR_kill   37
#define __NR_nosys   38
#define __NR_getppid   39
#define __NR_dup   41
#define __NR_pipe   42
#define __NR_getegid   43
#define __NR_sigaction   46
#define __NR_getgid   47
#define __NR_sigprocmask   48
#define __NR_getlogin   49
#define __NR_setlogin   50
#define __NR_acct   51
#define __NR_sigpending   52
#define __NR_sigaltstack   53
#define __NR_ioctl   54
#define __NR_reboot   55
#define __NR_revoke   56
#define __NR_symlink   57
#define __NR_readlink   58
#define __NR_execve   59
#define __NR_umask   60
#define __NR_chroot   61
#define __NR_msync   65
#define __NR_vfork   66
#define __NR_munmap   73
#define __NR_mprotect   74
#define __NR_madvise   75
#define __NR_mincore   78
#define __NR_getgroups   79
#define __NR_setgroups   80
#define __NR_getpgrp   81
#define __NR_setpgid   82
#define __NR_setitimer   83
#define __NR_swapon   85
#define __NR_getitimer   86
#define __NR_getdtablesize   89
#define __NR_dup2   90
#define __NR_fcntl   92
#define __NR_select   93
#define __NR_fsync   95
#define __NR_setpriority   96
#if SOCKETS
#define __NR_socket   97
#define __NR_connect   98
#endif /* SOCKETS */
#define __NR_getpriority   100
#if SOCKETS
#define __NR_bind   104
#define __NR_setsockopt   105
#define __NR_listen   106
#endif /* SOCKETS */
#define __NR_sigsuspend 111
#define __NR_gettimeofday 116
#define __NR_getrusage 117
#if SOCKETS
#define __NR_getsockopt 118
#endif /* SOCKETS */
#define __NR_readv   120
#define __NR_writev   121
#define __NR_settimeofday   122
#define __NR_fchown   123
#define __NR_fchmod   124
#define __NR_setreuid   126
#define __NR_setregid   127
#define __NR_rename   128
#define __NR_flock   131
#define __NR_mkfifo   132
#if SOCKETS
#define __NR_sendto 133
#define __NR_shutdown 134
#define __NR_socketpair 135
#endif /* SOCKETS */
#define __NR_mkdir 136
#define __NR_rmdir 137
#define __NR_utimes 138
#define __NR_futimes 139
#define __NR_adjtime 140
#define __NR_gethostuuid 142
#define __NR_setsid 147
#define __NR_getpgid 151
#define __NR_setprivexec 152
#define __NR_pread 153
#define __NR_pwrite 154

#if NFSSERVER
#define _NR__nfssvc 155
#endif

#define __NR_statfs 157
#define __NR_fstatfs 158
#define __NR_unmount 159

#if NFSSERVER
#define __NR_getfh 161
#endif

#define __NR_quotactl 165
#define __NR_mount 167
#define __NR_csops 169
#define __NR_csops_audittoken 170
#define __NR_waitid 173
#define __NR_kdebug_trace64 179
#define __NR_kdebug_trace 180
#define __NR_setgid 181
#define __NR_setegid 182
#define __NR_seteuid 183
#define __NR_sigreturn 184
#define __NR_chud 185
#define __NR_fdatasync 187
#define __NR_stat 188
#define __NR_fstat 189
#define __NR_lstat 190
#define __NR_pathconf 191
#define __NR_fpathconf 192
#define __NR_getrlimit 194
#define __NR_setrlimit 195
#define __NR_getdirentries 196
#define __NR_mmap 197
#define __NR_lseek 199
#define __NR_truncate 200
#define __NR_ftruncate 201
#define __NR_sysctl 202
#define __NR_mlock 203
#define __NR_munlock 204
#define __NR_undelete 205
#define __NR_open_dprotected_np 216
#define __NR_getattrlist 220
#define __NR_setattrlist 221
#define __NR_getdirentriesattr 222
#define __NR_exchangedata 223
#define __NR_searchfs 225
#define __NR_delete 226
#define __NR_copyfile 227
#define __NR_fgetattrlist 228
#define __NR_fsetattrlist 229
#define __NR_poll 230
#define __NR_watchevent 231
#define __NR_waitevent 232
#define __NR_modwatch 233
#define __NR_getxattr 234
#define __NR_fgetxattr 235
#define __NR_setxattr 236
#define __NR_fsetxattr 237
#define __NR_removexattr 238
#define __NR_fremovexattr 239
#define __NR_listxattr 240
#define __NR_flistxattr 241
#define __NR_fsctl 242
#define __NR_initgroups 243
#define __NR_posix_spawn 244
#define __NR_ffsctl 245

#if NFSCLIENT
#define __NR_nfsclnt 247
#endif

#if 0

#if NFSSERVER
248	AUE_FHOPEN	ALL	{ int fhopen(const struct fhandle *u_fhp, int flags); } 
#else
248	AUE_NULL	ALL	{ int nosys(void); } 
#endif

249	AUE_NULL	ALL	{ int nosys(void); } 
250	AUE_MINHERIT	ALL	{ int minherit(void *addr, size_t len, int inherit); } 
#if SYSV_SEM
251	AUE_SEMSYS	ALL	{ int semsys(u_int which, int a2, int a3, int a4, int a5) NO_SYSCALL_STUB; } 
#else
251	AUE_NULL	ALL	{ int nosys(void); } 
#endif
#if SYSV_MSG
252	AUE_MSGSYS	ALL	{ int msgsys(u_int which, int a2, int a3, int a4, int a5) NO_SYSCALL_STUB; }
#else
252	AUE_NULL	ALL	{ int nosys(void); } 
#endif
#if SYSV_SHM
253	AUE_SHMSYS	ALL	{ int shmsys(u_int which, int a2, int a3, int a4) NO_SYSCALL_STUB; } 
#else
253	AUE_NULL	ALL	{ int nosys(void); } 
#endif
#if SYSV_SEM
254	AUE_SEMCTL	ALL	{ int semctl(int semid, int semnum, int cmd, semun_t arg) NO_SYSCALL_STUB; } 
255	AUE_SEMGET	ALL	{ int semget(key_t key, int	nsems, int semflg); } 
256	AUE_SEMOP	ALL	{ int semop(int semid, struct sembuf *sops, int nsops); } 
257	AUE_NULL	ALL	{ int nosys(void); } 
#else
254	AUE_NULL	ALL	{ int nosys(void); } 
255	AUE_NULL	ALL	{ int nosys(void); } 
256	AUE_NULL	ALL	{ int nosys(void); } 
257	AUE_NULL	ALL	{ int nosys(void); } 
#endif
#if SYSV_MSG
258	AUE_MSGCTL	ALL	{ int msgctl(int msqid, int cmd, struct	msqid_ds *buf) NO_SYSCALL_STUB; } 
259	AUE_MSGGET	ALL	{ int msgget(key_t key, int msgflg); } 
260	AUE_MSGSND	ALL	{ int msgsnd(int msqid, void *msgp, size_t msgsz, int msgflg); } 
261	AUE_MSGRCV	ALL	{ user_ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg); } 
#else
258	AUE_NULL	ALL	{ int nosys(void); } 
259	AUE_NULL	ALL	{ int nosys(void); } 
260	AUE_NULL	ALL	{ int nosys(void); } 
261	AUE_NULL	ALL	{ int nosys(void); } 
#endif
#if SYSV_SHM
262	AUE_SHMAT	ALL	{ user_addr_t shmat(int shmid, void *shmaddr, int shmflg); } 
263	AUE_SHMCTL	ALL	{ int shmctl(int shmid, int cmd, struct shmid_ds *buf) NO_SYSCALL_STUB; } 
264	AUE_SHMDT	ALL	{ int shmdt(void *shmaddr); } 
265	AUE_SHMGET	ALL	{ int shmget(key_t key, size_t size, int shmflg); } 
#else
262	AUE_NULL	ALL	{ int nosys(void); } 
263	AUE_NULL	ALL	{ int nosys(void); } 
264	AUE_NULL	ALL	{ int nosys(void); } 
265	AUE_NULL	ALL	{ int nosys(void); } 
#endif
266	AUE_SHMOPEN	ALL	{ int shm_open(const char *name, int oflag, int mode) NO_SYSCALL_STUB; } 
267	AUE_SHMUNLINK	ALL	{ int shm_unlink(const char *name); } 
268	AUE_SEMOPEN	ALL	{ user_addr_t sem_open(const char *name, int oflag, int mode, int value) NO_SYSCALL_STUB; } 
269	AUE_SEMCLOSE	ALL	{ int sem_close(sem_t *sem); } 
270	AUE_SEMUNLINK	ALL	{ int sem_unlink(const char *name); } 
271	AUE_SEMWAIT	ALL	{ int sem_wait(sem_t *sem); } 
272	AUE_SEMTRYWAIT	ALL	{ int sem_trywait(sem_t *sem); } 
273	AUE_SEMPOST	ALL	{ int sem_post(sem_t *sem); } 
274	AUE_SYSCTL	ALL 	{ int sysctlbyname(const char *name, size_t namelen, void *old, size_t *oldlenp, void *new, size_t newlen) NO_SYSCALL_STUB; }
275	AUE_NULL	ALL	{ int enosys(void); } { old sem_init }
276	AUE_NULL	ALL	{ int enosys(void); } { old sem_destroy }
277	AUE_OPEN_EXTENDED_RWTC	ALL	{ int open_extended(user_addr_t path, int flags, uid_t uid, gid_t gid, int mode, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
278	AUE_UMASK_EXTENDED	ALL	{ int umask_extended(int newmask, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
279	AUE_STAT_EXTENDED	ALL	{ int stat_extended(user_addr_t path, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
280	AUE_LSTAT_EXTENDED	ALL	{ int lstat_extended(user_addr_t path, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
281	AUE_FSTAT_EXTENDED	ALL	{ int fstat_extended(int fd, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
282	AUE_CHMOD_EXTENDED	ALL	{ int chmod_extended(user_addr_t path, uid_t uid, gid_t gid, int mode, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
283	AUE_FCHMOD_EXTENDED	ALL	{ int fchmod_extended(int fd, uid_t uid, gid_t gid, int mode, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
284	AUE_ACCESS_EXTENDED	ALL	{ int access_extended(user_addr_t entries, size_t size, user_addr_t results, uid_t uid) NO_SYSCALL_STUB; } 
285	AUE_SETTID	ALL	{ int settid(uid_t uid, gid_t gid) NO_SYSCALL_STUB; } 
286	AUE_GETTID	ALL	{ int gettid(uid_t *uidp, gid_t *gidp) NO_SYSCALL_STUB; } 
287	AUE_SETSGROUPS	ALL	{ int setsgroups(int setlen, user_addr_t guidset) NO_SYSCALL_STUB; } 
288	AUE_GETSGROUPS	ALL	{ int getsgroups(user_addr_t setlen, user_addr_t guidset) NO_SYSCALL_STUB; } 
289	AUE_SETWGROUPS	ALL	{ int setwgroups(int setlen, user_addr_t guidset) NO_SYSCALL_STUB; } 
290	AUE_GETWGROUPS	ALL	{ int getwgroups(user_addr_t setlen, user_addr_t guidset) NO_SYSCALL_STUB; }
291	AUE_MKFIFO_EXTENDED	ALL	{ int mkfifo_extended(user_addr_t path, uid_t uid, gid_t gid, int mode, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
292	AUE_MKDIR_EXTENDED	ALL	{ int mkdir_extended(user_addr_t path, uid_t uid, gid_t gid, int mode, user_addr_t xsecurity) NO_SYSCALL_STUB; } 
#if CONFIG_EXT_RESOLVER
293	AUE_IDENTITYSVC	ALL	{ int identitysvc(int opcode, user_addr_t message) NO_SYSCALL_STUB; } 
#else
293	AUE_NULL	ALL	{ int nosys(void); } 
#endif
294	AUE_NULL	ALL	{ int shared_region_check_np(uint64_t *start_address) NO_SYSCALL_STUB; }
295	AUE_NULL	ALL	{ int nosys(void); } { old shared_region_map_np }
296	AUE_NULL	ALL	{ int vm_pressure_monitor(int wait_for_pressure, int nsecs_monitored, uint32_t *pages_reclaimed); }
#if PSYNCH
297	AUE_NULL	ALL	{ uint32_t psynch_rw_longrdlock(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
298	AUE_NULL	ALL	{ uint32_t psynch_rw_yieldwrlock(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
299	AUE_NULL	ALL	{ int psynch_rw_downgrade(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
300	AUE_NULL	ALL	{ uint32_t psynch_rw_upgrade(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
301	AUE_NULL	ALL	{ uint32_t psynch_mutexwait(user_addr_t mutex,  uint32_t mgen, uint32_t  ugen, uint64_t tid, uint32_t flags) NO_SYSCALL_STUB; }
302	AUE_NULL	ALL	{ uint32_t psynch_mutexdrop(user_addr_t mutex,  uint32_t mgen, uint32_t  ugen, uint64_t tid, uint32_t flags) NO_SYSCALL_STUB; }
303	AUE_NULL	ALL	{ uint32_t psynch_cvbroad(user_addr_t cv, uint64_t cvlsgen, uint64_t cvudgen, uint32_t flags, user_addr_t mutex,  uint64_t mugen, uint64_t tid) NO_SYSCALL_STUB; }
304	AUE_NULL	ALL	{ uint32_t psynch_cvsignal(user_addr_t cv, uint64_t cvlsgen, uint32_t cvugen, int thread_port, user_addr_t mutex,  uint64_t mugen, uint64_t tid, uint32_t flags) NO_SYSCALL_STUB; }
305	AUE_NULL	ALL	{ uint32_t psynch_cvwait(user_addr_t cv, uint64_t cvlsgen, uint32_t cvugen, user_addr_t mutex,  uint64_t mugen, uint32_t flags, int64_t sec, uint32_t nsec) NO_SYSCALL_STUB; }
306	AUE_NULL	ALL	{ uint32_t psynch_rw_rdlock(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
307	AUE_NULL	ALL	{ uint32_t psynch_rw_wrlock(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
308	AUE_NULL	ALL	{ uint32_t psynch_rw_unlock(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
309	AUE_NULL	ALL	{ uint32_t psynch_rw_unlock2(user_addr_t rwlock, uint32_t lgenval, uint32_t ugenval, uint32_t rw_wc, int flags)  NO_SYSCALL_STUB; }
#else
297	AUE_NULL	ALL	{ int nosys(void); } { old reset_shared_file }
298	AUE_NULL	ALL	{ int nosys(void); } { old new_system_shared_regions }
299	AUE_NULL	ALL	{ int enosys(void); } { old shared_region_map_file_np }
300	AUE_NULL	ALL	{ int enosys(void); } { old shared_region_make_private_np }
301	AUE_NULL	ALL	{ int nosys(void); } 
302	AUE_NULL	ALL	{ int nosys(void); }
303	AUE_NULL	ALL	{ int nosys(void); }
304	AUE_NULL	ALL	{ int nosys(void); }
305	AUE_NULL	ALL	{ int nosys(void); }
306	AUE_NULL	ALL	{ int nosys(void); }
307	AUE_NULL	ALL	{ int nosys(void); }
308	AUE_NULL	ALL	{ int nosys(void); }
309	AUE_NULL	ALL	{ int nosys(void); }
#endif
310	AUE_GETSID	ALL	{ int getsid(pid_t pid); } 
311	AUE_SETTIDWITHPID	ALL	{ int settid_with_pid(pid_t pid, int assume) NO_SYSCALL_STUB; } 
#if PSYNCH
312	AUE_NULL	ALL	{ int psynch_cvclrprepost(user_addr_t cv, uint32_t cvgen, uint32_t cvugen, uint32_t cvsgen, uint32_t prepocnt, uint32_t preposeq, uint32_t flags) NO_SYSCALL_STUB; }
#else
312	AUE_NULL	ALL	{ int nosys(void); } { old __pthread_cond_timedwait }
#endif
313	AUE_NULL	ALL	{ int aio_fsync(int op, user_addr_t aiocbp); } 
314 	AUE_NULL	ALL	{ user_ssize_t aio_return(user_addr_t aiocbp); } 
315	AUE_NULL	ALL	{ int aio_suspend(user_addr_t aiocblist, int nent, user_addr_t timeoutp); } 
316	AUE_NULL	ALL	{ int aio_cancel(int fd, user_addr_t aiocbp); } 
317	AUE_NULL	ALL	{ int aio_error(user_addr_t aiocbp); } 
318	AUE_NULL	ALL	{ int aio_read(user_addr_t aiocbp); } 
319	AUE_NULL	ALL	{ int aio_write(user_addr_t aiocbp); } 
320	AUE_LIOLISTIO	ALL	{ int lio_listio(int mode, user_addr_t aiocblist, int nent, user_addr_t sigp); } 
321	AUE_NULL	ALL	{ int nosys(void); } { old __pthread_cond_wait }
322	AUE_IOPOLICYSYS	ALL	{ int iopolicysys(int cmd, void *arg) NO_SYSCALL_STUB; } 
323	AUE_NULL	ALL	{ int process_policy(int scope, int action, int policy, int policy_subtype, user_addr_t attrp, pid_t target_pid, uint64_t target_threadid) NO_SYSCALL_STUB; } 
324	AUE_MLOCKALL	ALL	{ int mlockall(int how); } 
325	AUE_MUNLOCKALL	ALL	{ int munlockall(int how); } 
326	AUE_NULL	ALL	{ int nosys(void); } 
327	AUE_ISSETUGID	ALL	{ int issetugid(void); } 
328	AUE_PTHREADKILL	ALL	{ int __pthread_kill(int thread_port, int sig); } 
329	AUE_PTHREADSIGMASK	ALL	{ int __pthread_sigmask(int how, user_addr_t set, user_addr_t oset); } 
330	AUE_SIGWAIT	ALL	{ int __sigwait(user_addr_t set, user_addr_t sig); } 
331	AUE_NULL	ALL	{ int __disable_threadsignal(int value); } 
332	AUE_NULL	ALL	{ int __pthread_markcancel(int thread_port); } 
333	AUE_NULL	ALL	{ int __pthread_canceled(int  action); } 

;#if OLD_SEMWAIT_SIGNAL
;334	AUE_NULL	ALL	{ int nosys(void); }   { old __semwait_signal }
;#else
334     AUE_SEMWAITSIGNAL       ALL     { int __semwait_signal(int cond_sem, int mutex_sem, int timeout, int relative, int64_t tv_sec, int32_t tv_nsec); }
;#endif

335	AUE_NULL	ALL	{ int nosys(void); }   { old utrace }
336	AUE_PROCINFO	ALL	{ int proc_info(int32_t callnum,int32_t pid,uint32_t flavor, uint64_t arg,user_addr_t buffer,int32_t buffersize) NO_SYSCALL_STUB; } 
#if SENDFILE
337	AUE_SENDFILE	ALL	{ int sendfile(int fd, int s, off_t offset, off_t *nbytes, struct sf_hdtr *hdtr, int flags); } 
#else /* !SENDFILE */
337	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SENDFILE */
338	AUE_STAT64	ALL	{ int stat64(user_addr_t path, user_addr_t ub); } 
339	AUE_FSTAT64	ALL	{ int fstat64(int fd, user_addr_t ub); } 
340	AUE_LSTAT64	ALL	{ int lstat64(user_addr_t path, user_addr_t ub); } 
341	AUE_STAT64_EXTENDED	ALL	{ int stat64_extended(user_addr_t path, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
342	AUE_LSTAT64_EXTENDED	ALL	{ int lstat64_extended(user_addr_t path, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
343	AUE_FSTAT64_EXTENDED	ALL	{ int fstat64_extended(int fd, user_addr_t ub, user_addr_t xsecurity, user_addr_t xsecurity_size) NO_SYSCALL_STUB; } 
344	AUE_GETDIRENTRIES64	ALL	{ user_ssize_t getdirentries64(int fd, void *buf, user_size_t bufsize, off_t *position) NO_SYSCALL_STUB; } 
345	AUE_STATFS64	ALL	{ int statfs64(char *path, struct statfs64 *buf); } 
346	AUE_FSTATFS64	ALL	{ int fstatfs64(int fd, struct statfs64 *buf); } 
347	AUE_GETFSSTAT64	ALL	{ int getfsstat64(user_addr_t buf, int bufsize, int flags); } 
348	AUE_NULL	ALL	{ int __pthread_chdir(user_addr_t path); } 
349	AUE_NULL	ALL	{ int __pthread_fchdir(int fd); } 
350	AUE_AUDIT	ALL	{ int audit(void *record, int length); } 
351	AUE_AUDITON	ALL	{ int auditon(int cmd, void *data, int length); } 
352	AUE_NULL	ALL	{ int nosys(void); } 
353	AUE_GETAUID	ALL	{ int getauid(au_id_t *auid); } 
354	AUE_SETAUID	ALL	{ int setauid(au_id_t *auid); } 
355	AUE_NULL	ALL	{ int nosys(void); }	{ old getaudit }
356	AUE_NULL	ALL	{ int nosys(void); }	{ old setaudit }
357	AUE_GETAUDIT_ADDR	ALL	{ int getaudit_addr(struct auditinfo_addr *auditinfo_addr, int length); } 
358	AUE_SETAUDIT_ADDR	ALL	{ int setaudit_addr(struct auditinfo_addr *auditinfo_addr, int length); } 
359	AUE_AUDITCTL	ALL	{ int auditctl(char *path); } 
#if CONFIG_WORKQUEUE
360	AUE_NULL	ALL	{ user_addr_t bsdthread_create(user_addr_t func, user_addr_t func_arg, user_addr_t stack, user_addr_t pthread, uint32_t flags) NO_SYSCALL_STUB; } 
361	AUE_NULL	ALL	{ int bsdthread_terminate(user_addr_t stackaddr, size_t freesize, uint32_t port, uint32_t sem) NO_SYSCALL_STUB; } 
#else
360	AUE_NULL	ALL	{ int nosys(void); } 
361	AUE_NULL	ALL	{ int nosys(void); } 
#endif /* CONFIG_WORKQUEUE */
362	AUE_KQUEUE	ALL	{ int kqueue(void); } 
363	AUE_NULL	ALL	{ int kevent(int fd, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout); } 
364	AUE_LCHOWN	ALL	{ int lchown(user_addr_t path, uid_t owner, gid_t group) NO_SYSCALL_STUB; }
365	AUE_STACKSNAPSHOT	ALL	{ int stack_snapshot(pid_t pid, user_addr_t tracebuf, uint32_t tracebuf_size, uint32_t flags, uint32_t dispatch_offset) NO_SYSCALL_STUB; }
#if CONFIG_WORKQUEUE
366	AUE_NULL	ALL	{ int bsdthread_register(user_addr_t threadstart, user_addr_t wqthread, uint32_t flags, user_addr_t stack_addr_hint, user_addr_t targetconc_ptr, uint32_t dispatchqueue_offset, uint32_t tsd_offset) NO_SYSCALL_STUB; } 
367	AUE_WORKQOPEN	ALL	{ int workq_open(void) NO_SYSCALL_STUB; }
368	AUE_WORKQOPS	ALL	{ int workq_kernreturn(int options, user_addr_t item, int affinity, int prio) NO_SYSCALL_STUB; }
#else
366	AUE_NULL	ALL	{ int nosys(void); } 
367	AUE_NULL	ALL	{ int nosys(void); } 
368	AUE_NULL	ALL	{ int nosys(void); } 
#endif /* CONFIG_WORKQUEUE */
369	AUE_NULL	ALL	{ int kevent64(int fd, const struct kevent64_s *changelist, int nchanges, struct kevent64_s *eventlist, int nevents, unsigned int flags, const struct timespec *timeout); } 
#if OLD_SEMWAIT_SIGNAL
370     AUE_SEMWAITSIGNAL       ALL     { int __old_semwait_signal(int cond_sem, int mutex_sem, int timeout, int relative, const struct timespec *ts); }
371     AUE_SEMWAITSIGNAL       ALL     { int __old_semwait_signal_nocancel(int cond_sem, int mutex_sem, int timeout, int relative, const struct timespec *ts) NO_SYSCALL_STUB; }
#else
370     AUE_NULL        ALL     { int nosys(void); }   { old __semwait_signal }
371     AUE_NULL        ALL     { int nosys(void); }   { old __semwait_signal }
#endif
372	AUE_NULL	ALL	{ uint64_t thread_selfid (void) NO_SYSCALL_STUB; } 
373	AUE_LEDGER	ALL	{ int ledger(int cmd, caddr_t arg1, caddr_t arg2, caddr_t arg3); } 
374	AUE_NULL	ALL	{ int nosys(void); } 
375	AUE_NULL	ALL	{ int nosys(void); } 
376	AUE_NULL	ALL	{ int nosys(void); } 
377	AUE_NULL	ALL	{ int nosys(void); } 
378	AUE_NULL	ALL	{ int nosys(void); } 
379	AUE_NULL	ALL	{ int nosys(void); } 
380	AUE_MAC_EXECVE	ALL	{ int __mac_execve(char *fname, char **argp, char **envp, struct mac *mac_p); } 
#if CONFIG_MACF
381	AUE_MAC_SYSCALL	ALL	{ int __mac_syscall(char *policy, int call, user_addr_t arg); } 
382	AUE_MAC_GET_FILE	ALL	{ int __mac_get_file(char *path_p, struct mac *mac_p); } 
383	AUE_MAC_SET_FILE	ALL	{ int __mac_set_file(char *path_p, struct mac *mac_p); } 
384	AUE_MAC_GET_LINK	ALL	{ int __mac_get_link(char *path_p, struct mac *mac_p); } 
385	AUE_MAC_SET_LINK	ALL	{ int __mac_set_link(char *path_p, struct mac *mac_p); } 
386	AUE_MAC_GET_PROC	ALL	{ int __mac_get_proc(struct mac *mac_p); } 
387	AUE_MAC_SET_PROC	ALL	{ int __mac_set_proc(struct mac *mac_p); } 
388	AUE_MAC_GET_FD	ALL	{ int __mac_get_fd(int fd, struct mac *mac_p); } 
389	AUE_MAC_SET_FD	ALL	{ int __mac_set_fd(int fd, struct mac *mac_p); } 
390	AUE_MAC_GET_PID	ALL	{ int __mac_get_pid(pid_t pid, struct mac *mac_p); } 
391	AUE_MAC_GET_LCID	ALL	{ int __mac_get_lcid(pid_t lcid, struct mac *mac_p); }
392	AUE_MAC_GET_LCTX	ALL	{ int __mac_get_lctx(struct mac *mac_p); }
393	AUE_MAC_SET_LCTX	ALL	{ int __mac_set_lctx(struct mac *mac_p); }
#else
381     AUE_MAC_SYSCALL ALL     { int enosys(void); }
382     AUE_MAC_GET_FILE        ALL     { int nosys(void); }
383     AUE_MAC_SET_FILE        ALL     { int nosys(void); }
384     AUE_MAC_GET_LINK        ALL     { int nosys(void); }
385     AUE_MAC_SET_LINK        ALL     { int nosys(void); }
386     AUE_MAC_GET_PROC        ALL     { int nosys(void); }
387     AUE_MAC_SET_PROC        ALL     { int nosys(void); }
388     AUE_MAC_GET_FD  ALL     { int nosys(void); }
389     AUE_MAC_SET_FD  ALL     { int nosys(void); }
390     AUE_MAC_GET_PID ALL     { int nosys(void); }
391     AUE_MAC_GET_LCID        ALL     { int nosys(void); }
392     AUE_MAC_GET_LCTX        ALL     { int nosys(void); }
393     AUE_MAC_SET_LCTX        ALL     { int nosys(void); }
#endif
394	AUE_SETLCID	ALL	{ int setlcid(pid_t pid, pid_t lcid) NO_SYSCALL_STUB; }
395	AUE_GETLCID	ALL	{ int getlcid(pid_t pid) NO_SYSCALL_STUB; }
396	AUE_NULL	ALL	{ user_ssize_t read_nocancel(int fd, user_addr_t cbuf, user_size_t nbyte) NO_SYSCALL_STUB; } 
397	AUE_NULL	ALL	{ user_ssize_t write_nocancel(int fd, user_addr_t cbuf, user_size_t nbyte) NO_SYSCALL_STUB; } 
398	AUE_OPEN_RWTC	ALL	{ int open_nocancel(user_addr_t path, int flags, int mode) NO_SYSCALL_STUB; } 
399	AUE_CLOSE	ALL	{ int close_nocancel(int fd) NO_SYSCALL_STUB; } 
400	AUE_WAIT4	ALL	{ int wait4_nocancel(int pid, user_addr_t status, int options, user_addr_t rusage) NO_SYSCALL_STUB; } 
#if SOCKETS
401	AUE_RECVMSG	ALL	{ int recvmsg_nocancel(int s, struct msghdr *msg, int flags) NO_SYSCALL_STUB; } 
402	AUE_SENDMSG	ALL	{ int sendmsg_nocancel(int s, caddr_t msg, int flags) NO_SYSCALL_STUB; } 
403	AUE_RECVFROM	ALL	{ int recvfrom_nocancel(int s, void *buf, size_t len, int flags, struct sockaddr *from, int *fromlenaddr) NO_SYSCALL_STUB; } 
404	AUE_ACCEPT	ALL	{ int accept_nocancel(int s, caddr_t name, socklen_t	*anamelen) NO_SYSCALL_STUB; } 
#else
401	AUE_NULL	ALL	{ int nosys(void); }
402	AUE_NULL	ALL	{ int nosys(void); }
403	AUE_NULL	ALL	{ int nosys(void); }
404	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SOCKETS */
405	AUE_MSYNC	ALL	{ int msync_nocancel(caddr_t addr, size_t len, int flags) NO_SYSCALL_STUB; } 
406	AUE_FCNTL	ALL	{ int fcntl_nocancel(int fd, int cmd, long arg) NO_SYSCALL_STUB; } 
407	AUE_SELECT	ALL	{ int select_nocancel(int nd, u_int32_t *in, u_int32_t *ou, u_int32_t *ex, struct timeval *tv) NO_SYSCALL_STUB; } 
408	AUE_FSYNC	ALL	{ int fsync_nocancel(int fd) NO_SYSCALL_STUB; } 
#if SOCKETS
409	AUE_CONNECT	ALL	{ int connect_nocancel(int s, caddr_t name, socklen_t namelen) NO_SYSCALL_STUB; } 
#else
409	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SOCKETS */
410	AUE_NULL	ALL	{ int sigsuspend_nocancel(sigset_t mask) NO_SYSCALL_STUB; } 
411	AUE_READV	ALL	{ user_ssize_t readv_nocancel(int fd, struct iovec *iovp, u_int iovcnt) NO_SYSCALL_STUB; } 
412	AUE_WRITEV	ALL	{ user_ssize_t writev_nocancel(int fd, struct iovec *iovp, u_int iovcnt) NO_SYSCALL_STUB; } 
#if SOCKETS
413	AUE_SENDTO	ALL	{ int sendto_nocancel(int s, caddr_t buf, size_t len, int flags, caddr_t to, socklen_t tolen) NO_SYSCALL_STUB; } 
#else
413	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SOCKETS */
414	AUE_PREAD	ALL	{ user_ssize_t pread_nocancel(int fd, user_addr_t buf, user_size_t nbyte, off_t offset) NO_SYSCALL_STUB; } 
415	AUE_PWRITE	ALL	{ user_ssize_t pwrite_nocancel(int fd, user_addr_t buf, user_size_t nbyte, off_t offset) NO_SYSCALL_STUB; } 
416	AUE_WAITID	ALL	{ int waitid_nocancel(idtype_t idtype, id_t id, siginfo_t *infop, int options) NO_SYSCALL_STUB; } 
417	AUE_POLL	ALL	{ int poll_nocancel(struct pollfd *fds, u_int nfds, int timeout) NO_SYSCALL_STUB; } 
#if SYSV_MSG
418	AUE_MSGSND	ALL	{ int msgsnd_nocancel(int msqid, void *msgp, size_t msgsz, int msgflg) NO_SYSCALL_STUB; } 
419	AUE_MSGRCV	ALL	{ user_ssize_t msgrcv_nocancel(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) NO_SYSCALL_STUB; } 
#else
418	AUE_NULL	ALL	{ int nosys(void); } 
419	AUE_NULL	ALL	{ int nosys(void); } 
#endif
420	AUE_SEMWAIT	ALL	{ int sem_wait_nocancel(sem_t *sem) NO_SYSCALL_STUB; } 
421	AUE_NULL	ALL	{ int aio_suspend_nocancel(user_addr_t aiocblist, int nent, user_addr_t timeoutp) NO_SYSCALL_STUB; } 
422	AUE_SIGWAIT	ALL	{ int __sigwait_nocancel(user_addr_t set, user_addr_t sig) NO_SYSCALL_STUB; } 
;#if OLD_SEMWAIT_SIGNAL 
;423	AUE_NULL	ALL	{ int nosys(void); }   { old __semwait_signal_nocancel }
;#else
423     AUE_SEMWAITSIGNAL       ALL     { int __semwait_signal_nocancel(int cond_sem, int mutex_sem, int timeout, int relative, int64_t tv_sec, int32_t tv_nsec); }
;#endif
424	AUE_MAC_MOUNT	ALL	{ int __mac_mount(char *type, char *path, int flags, caddr_t data, struct mac *mac_p); } 
#if CONFIG_MACF
425	AUE_MAC_GET_MOUNT	ALL	{ int __mac_get_mount(char *path, struct mac *mac_p); } 
#else
425     AUE_MAC_GET_MOUNT       ALL     { int nosys(void); }
#endif
426	AUE_MAC_GETFSSTAT	ALL	{ int __mac_getfsstat(user_addr_t buf, int bufsize, user_addr_t mac, int macsize, int flags); } 
427	AUE_FSGETPATH	ALL	{ user_ssize_t fsgetpath(user_addr_t buf, size_t bufsize, user_addr_t fsid, uint64_t objid) NO_SYSCALL_STUB; } { private fsgetpath (File Manager SPI) }
428	AUE_NULL	ALL	{ mach_port_name_t audit_session_self(void); }
429	AUE_NULL	ALL	{ int audit_session_join(mach_port_name_t port); }
430	AUE_NULL	ALL	{ int fileport_makeport(int fd, user_addr_t portnamep); }
431	AUE_NULL	ALL	{ int fileport_makefd(mach_port_name_t port); }
432	AUE_NULL	ALL	{ int audit_session_port(au_asid_t asid, user_addr_t portnamep); }
433	AUE_NULL	ALL	{ int pid_suspend(int pid); }
434	AUE_NULL	ALL	{ int pid_resume(int pid); }
435     AUE_NULL        ALL     { int nosys(void); }
436	AUE_NULL	ALL	{ int nosys(void); }
437	AUE_NULL	ALL	{ int nosys(void); } { old shared_region_slide_np }
438	AUE_NULL	ALL	{ int shared_region_map_and_slide_np(int fd, uint32_t count, const struct shared_file_mapping_np *mappings, uint32_t slide, uint64_t* slide_start, uint32_t slide_size) NO_SYSCALL_STUB; }
439	AUE_NULL	ALL	{ int kas_info(int selector, void *value, size_t *size); }
#if CONFIG_MEMORYSTATUS
440	AUE_NULL	ALL	{ int memorystatus_control(uint32_t command, int32_t pid, uint32_t flags, user_addr_t buffer, size_t buffersize); } 
#else
440	AUE_NULL	ALL	{ int nosys(void); }
#endif
441	AUE_OPEN_RWTC	ALL	{ int guarded_open_np(const char *path, const guardid_t *guard, u_int guardflags, int flags, int mode) NO_SYSCALL_STUB; }
442	AUE_CLOSE	ALL	{ int guarded_close_np(int fd, const guardid_t *guard); }
443	AUE_KQUEUE	ALL	{ int guarded_kqueue_np(const guardid_t *guard, u_int guardflags); }
444	AUE_NULL	ALL	{ int change_fdguard_np(int fd, const guardid_t *guard, u_int guardflags, const guardid_t *nguard, u_int nguardflags, int *fdflagsp); }
445	AUE_NULL	ALL	{ int nosys(void); } { old __proc_suppress } 
446	AUE_NULL	ALL	{ int proc_rlimit_control(pid_t pid, int flavor, void *arg); }
#if SOCKETS
447	AUE_CONNECT	ALL	{ int connectx(int s, struct sockaddr *src, socklen_t srclen, struct sockaddr *dsts, socklen_t dstlen, uint32_t ifscope, associd_t aid, connid_t *cid); } 
448	AUE_NULL	ALL	{ int disconnectx(int s, associd_t aid, connid_t cid); } 
449	AUE_NULL	ALL	{ int peeloff(int s, associd_t aid); } 
450	AUE_SOCKET	ALL	{ int socket_delegate(int domain, int type, int protocol, pid_t epid); } 
#else
447	AUE_NULL	ALL	{ int nosys(void); }
448	AUE_NULL	ALL	{ int nosys(void); }
449	AUE_NULL	ALL	{ int nosys(void); }
450	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SOCKETS */
451	AUE_NULL	ALL	{ int telemetry(uint64_t cmd, uint64_t deadline, uint64_t interval, uint64_t leeway, uint64_t arg4, uint64_t arg5) NO_SYSCALL_STUB; } 
#if CONFIG_PROC_UUID_POLICY
452	AUE_NULL	ALL	{ int proc_uuid_policy(uint32_t operation, uuid_t uuid, size_t uuidlen, uint32_t flags); }
#else
452	AUE_NULL	ALL	{ int nosys(void); }
#endif
#if CONFIG_MEMORYSTATUS
453	AUE_NULL	ALL	{ int memorystatus_get_level(user_addr_t level); }
#else
453	AUE_NULL	ALL	{ int nosys(void); }
#endif
454	AUE_NULL	ALL	{ int system_override(uint64_t timeout, uint64_t flags); }
455	AUE_NULL	ALL	{ int vfs_purge(void); }
456	AUE_NULL	ALL	{ int sfi_ctl(uint32_t operation, uint32_t sfi_class, uint64_t time, uint64_t *out_time) NO_SYSCALL_STUB; }
457	AUE_NULL	ALL	{ int sfi_pidctl(uint32_t operation, pid_t pid, uint32_t sfi_flags, uint32_t *out_sfi_flags) NO_SYSCALL_STUB; }
#if CONFIG_COALITIONS
458	AUE_NULL	ALL	{ int coalition(uint32_t operation, uint64_t *cid, uint32_t flags) NO_SYSCALL_STUB; }
459	AUE_NULL	ALL	{ int coalition_info(uint32_t flavor, uint64_t *cid, void *buffer, size_t *bufsize) NO_SYSCALL_STUB; }
#else
458	AUE_NULL	ALL	{ int enosys(void); }
459	AUE_NULL	ALL	{ int enosys(void); }
#endif /* COALITIONS */
#if NECP
460 AUE_NULL	ALL { int necp_match_policy(uint8_t *parameters, size_t parameters_size, struct necp_aggregate_result *returned_result); }
#else
460	AUE_NULL	ALL	{ int nosys(void); }
#endif /* NECP */
461	AUE_GETATTRLISTBULK	ALL	{ int getattrlistbulk(int dirfd, struct attrlist *alist, void *attributeBuffer, size_t bufferSize, uint64_t options); }
462	AUE_NULL	ALL { int enosys(void); } /* PLACEHOLDER for CLONEFILE */
463	AUE_OPENAT_RWTC	ALL	{ int openat(int fd, user_addr_t path, int flags, int mode) NO_SYSCALL_STUB; }
464	AUE_OPENAT_RWTC	ALL	{ int openat_nocancel(int fd, user_addr_t path, int flags, int mode) NO_SYSCALL_STUB; }
465	AUE_RENAMEAT	ALL	{ int renameat(int fromfd, char *from, int tofd, char *to) NO_SYSCALL_STUB; }
466	AUE_FACCESSAT	ALL	{ int faccessat(int fd, user_addr_t path, int amode, int flag); }
467	AUE_FCHMODAT	ALL	{ int fchmodat(int fd, user_addr_t path, int mode, int flag); }
468	AUE_FCHOWNAT	ALL	{ int fchownat(int fd, user_addr_t path, uid_t uid,gid_t gid, int flag); }
469	AUE_FSTATAT	ALL	{ int fstatat(int fd, user_addr_t path, user_addr_t ub, int flag); }
470	AUE_FSTATAT	ALL	{ int fstatat64(int fd, user_addr_t path, user_addr_t ub, int flag); }
471	AUE_LINKAT	ALL	{ int linkat(int fd1, user_addr_t path, int fd2, user_addr_t link, int flag); }
472	AUE_UNLINKAT	ALL	{ int unlinkat(int fd, user_addr_t path, int flag) NO_SYSCALL_STUB; }
473	AUE_READLINKAT	ALL	{ int readlinkat(int fd, user_addr_t path, user_addr_t buf, size_t bufsize); }
474	AUE_SYMLINKAT	ALL	{ int symlinkat(user_addr_t *path1, int fd, user_addr_t path2); }
475	AUE_MKDIRAT	ALL	{ int mkdirat(int fd, user_addr_t path, int mode); }
476	AUE_GETATTRLISTAT	ALL	{ int getattrlistat(int fd, const char *path, struct attrlist *alist, void *attributeBuffer, size_t bufferSize, u_long options); } 
477	AUE_NULL	ALL	{ int proc_trace_log(pid_t pid, uint64_t uniqueid); } 
478	AUE_NULL	ALL	{ int bsdthread_ctl(user_addr_t cmd, user_addr_t arg1, user_addr_t arg2, user_addr_t arg3) NO_SYSCALL_STUB; }
479	AUE_OPENBYID_RWT	ALL	{ int openbyid_np(user_addr_t fsid, user_addr_t objid, int oflags); }
#if SOCKETS
480	AUE_NULL	ALL	{ user_ssize_t recvmsg_x(int s, struct msghdr_x *msgp, u_int cnt, int flags); }
481	AUE_NULL	ALL	{ user_ssize_t sendmsg_x(int s, struct msghdr_x *msgp, u_int cnt, int flags); }
#else
480	AUE_NULL	ALL	{ int nosys(void); }
481	AUE_NULL	ALL	{ int nosys(void); }
#endif /* SOCKETS */
482	AUE_NULL	ALL	{ uint64_t thread_selfusage(void) NO_SYSCALL_STUB; }
#if CONFIG_CSR
483	AUE_NULL	ALL	{ int csrctl(uint32_t op, user_addr_t useraddr, user_addr_t usersize) NO_SYSCALL_STUB; }
#else
483	AUE_NULL	ALL	{ int enosys(void); }
#endif /* CSR */
484	AUE_NULL	ALL	{ int guarded_open_dprotected_np(const char *path, const guardid_t *guard, u_int guardflags, int flags, int dpclass, int dpflags, int mode) NO_SYSCALL_STUB; }
485	AUE_NULL	ALL	{ user_ssize_t guarded_write_np(int fd, const guardid_t *guard, user_addr_t cbuf, user_size_t nbyte); }
486	AUE_PWRITE	ALL	{ user_ssize_t guarded_pwrite_np(int fd, const guardid_t *guard, user_addr_t buf, user_size_t nbyte, off_t offset); }
487	AUE_WRITEV	ALL	{ user_ssize_t guarded_writev_np(int fd, const guardid_t *guard, struct iovec *iovp, u_int iovcnt); }
#if CONFIG_SECLUDED_RENAME
488	AUE_RENAME	ALL	{ int rename_ext(char *from, char *to, u_int flags) NO_SYSCALL_STUB; }
#else
488	AUE_NULL	ALL	{ int enosys(void); }
#endif
#if CONFIG_CODE_DECRYPTION
489	AUE_MPROTECT	ALL	{ int mremap_encrypted(caddr_t addr, size_t len, uint32_t cryptid, uint32_t cputype, uint32_t cpusubtype); } 
#else
489	AUE_NULL	ALL	{ int enosys(void); }
#endif

#endif

#endif // UNISTD_H
