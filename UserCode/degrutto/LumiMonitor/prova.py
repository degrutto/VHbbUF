import os
#file to process
os.system("python runregparse.py > runlog.log")
os.system("cd /tmp/degrutto/")
os.system("wget --certificate ~/.globus/usercert.pem --private-key ~/.globus/nopassUserkey.pem --ca-directory /afs/cern.ch/project/gd/LCG-share/certificates https://cmsweb.cern.ch/dqm/offline/data/browse/ROOT/OfflineData/Run2010/Mu/0001370xx/DQM_V0001_R000137020__Mu__Run2010A-PromptReco-v2__RECO.root")
os.system("cd -")


   
