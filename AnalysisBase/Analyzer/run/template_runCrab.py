from CRABClient.UserUtilities import config
config = config()

config.General.requestName = '_requestName_'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '_psetName_'

config.Data.inputDataset = '_inputDataset_'
config.JobType.pyCfgParams = ['inputDataset=%s' % config.Data.inputDataset]

config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = _unitsPerJob_
config.Data.outLFNDirBase = '_outLFNDirBase_'
config.Data.publication = False

config.Site.storageSite = '_storageSite_'
