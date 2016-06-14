import avango.daemon

head_station0 = avango.daemon.Station('kinect-head-0')

skeleton0 = avango.daemon.SkeletonTrack()
skeleton0.port = "7000"
skeleton0.server = "127.0.0.1"

skeleton0.stations[0] = head_station0

avango.daemon.run([skeleton0])
