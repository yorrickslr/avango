import avango.daemon

joint_stations = {}
for i in range(25):
    joint_stations[i] = avango.daemon.Station('kinect-joint-{0}'.format(str(i)))

skeleton0 = avango.daemon.SkeletonTrack()
skeleton0.port = "7700"
skeleton0.server = "141.54.147.35"

for i in range(25):
    skeleton0.stations[i] = joint_stations[i]


avango.daemon.run([skeleton0])
