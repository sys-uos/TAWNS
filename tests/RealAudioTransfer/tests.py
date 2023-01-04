import os,sys

tracks = {}

tracks["node[4]_30.000810667128_0_node[6]_microphones[0].flac"] = 562235 
tracks["node[4]_30.002254375415_0_node[5]_microphones[0].flac"] = 564047
tracks["node[4]_30.005013987387_0_node[7]_microphones[0].flac"] = 564047
tracks["node[4]_30.000810667128_1_node[6]_microphones[1].flac"] = 562234

tracks["node[4]_30.002254375415_1_node[5]_microphones[1].flac"] = 564128
tracks["node[4]_30.005013987387_1_node[7]_microphones[1].flac"] = 564128
tracks["node[4]_30.004845751522_0_node[0]_microphones[0].flac"] = 555844
tracks["node[4]_30.004845751522_1_node[0]_microphones[1].flac"] = 555793

tracks["node[4]_60.000810667128_2_node[6]_microphones[0].flac"] = 552742
tracks["node[4]_60.002254375415_2_node[5]_microphones[0].flac"] = 552742
tracks["node[4]_60.005013987387_2_node[7]_microphones[0].flac"] = 552742
tracks["node[4]_60.004845751522_2_node[0]_microphones[0].flac"] = 552742

tracks["node[4]_60.000810667128_3_node[6]_microphones[1].flac"] = 552742
tracks["node[4]_60.002254375415_3_node[5]_microphones[1].flac"] = 552742
tracks["node[4]_60.005013987387_3_node[7]_microphones[1].flac"] = 552742
tracks["node[4]_60.004845751522_3_node[0]_microphones[1].flac"] = 552742

generated_files = {}
folder = "./testserver/"
counter = 0

if not os.path.isdir(folder):
    print("Please complete a full run of the Simulation first!")
    sys.exit()

for f in os.listdir(folder):
    counter+=1
    generated_files[f.split("_",1)[1]] = os.stat(folder + f).st_size

if counter > 16:
    print("Please delete or rename the testserver folder before starting a new test!")
    sys.exit()

print("Test1: Checking, whether all files were generated.")
for t in tracks:
    assert(t in generated_files), "track name: " + t
print("Test1: Successfull")

print("---------------")

print("Test2: Checking, if all files have the correct sizes.")        
for t in tracks:
    assert(tracks[t] == generated_files[t]), "track size: " + str(tracks[t]) + "generated file size: " + str(generated_files[t]) 
print("Test2: Successfull")    