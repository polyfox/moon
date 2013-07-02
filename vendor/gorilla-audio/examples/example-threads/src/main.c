#include "gorilla/ga.h"
#include "gorilla/gau.h"

#include <stdio.h>

static void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
{
  gc_int32* flag = (gc_int32*)(in_context);
  *flag = 1;
  ga_handle_destroy(in_handle);
}
int main(int argc, char** argv)
{
  gau_Manager* mgr;
  ga_Mixer* mixer;
  ga_StreamManager* streamMgr;
  ga_Handle* stream;
  gau_SampleSourceLoop* loopSrc = 0;
  gau_SampleSourceLoop** pLoopSrc = &loopSrc;
  gc_int32 loop = 0;
  gc_int32 quit = 0;

  /* Initialize library + manager */
  gc_initialize(0);
  mgr = gau_manager_create_custom(GA_DEVICE_TYPE_DEFAULT, GAU_THREAD_POLICY_MULTI, 4, 512);
  mixer = gau_manager_mixer(mgr);
  streamMgr = gau_manager_streamManager(mgr);

  /* Create and play streaming audio */
  if(!loop)
    pLoopSrc = 0;
  stream = gau_create_handle_buffered_file(mixer, streamMgr, "test.ogg", "ogg",
                                           &setFlagAndDestroyOnFinish, &quit, pLoopSrc);
  ga_handle_play(stream);

  /* Bounded mix/queue/dispatch loop */
  while(!quit)
  {
    gau_manager_update(mgr);
    printf("%d / %d\n", ga_handle_tell(stream, GA_TELL_PARAM_CURRENT), ga_handle_tell(stream, GA_TELL_PARAM_TOTAL));
    gc_thread_sleep(1);
  }

  /* Clean up library + manager */
  gau_manager_destroy(mgr);
  gc_shutdown();

  return 0;
}
