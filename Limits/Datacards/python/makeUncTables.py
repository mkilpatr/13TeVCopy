#! /usr/bin/python
import os
from math import sqrt
import sys

"""Makes the uncertanty tables for the AN for the various backgrounds using the template datacards. If the
uncertanty names change, the "sources" dictornary will need to be updated apropriately. Other backgrounds
can be added as new lists, just make sure that the numbers assigned to each bkg in the "ps" dictionary
is correct. 
"""

binsB   = (1,2)
binsMet = (250,300,400,500,600)
ps = {'ttbarplusw':3, 'znunu':4, 'ttz':5, 'qcd':6, 'onelepcr':3}
sources = {
  'ttbarplusw' : [('corr_l'               , 'Lepton veto'                ),
                  ('corr_tau'             , 'Tau veto'                   ),
                  ('eff_b_heavy'          , '\\bq-tagging: heavy flavor' ),
                  ('eff_b_light'          , '\\bq-tagging: light flavor' ),
                  ('pu'                   , 'Pileup reweighting'         ),
                  ('scale_j'              , 'Jet energy scale'           ),
                  ('ttbarNorm'            , '\\ttbar~normalization'      ),
                  ('wjetsNorm'            , '\\W+jets normalization'     ),
                  ('mcstat_ttbarplusw_bin', 'Monte Carlo statistics (SR)'),
                  ('lostlep_bin_onelepcr' , 'Data statistics (CR)'       ),
  ],
  'onelepcr' : [('corr_l'                        , 'Lepton veto'           ),
                ('mcstat_ttbarplusw_bin_onelepcr', 'Monte Carlo statistics'),
                ('lostlep_bin_onelepcr'          , 'Data statistics'       ),
  ],
  'znunu' : [('corr_l'                        , 'Lepton veto'                ),
             ('eff_b_heavy'                   , '\\bq-tagging: heavy flavor' ),
             ('eff_b_light'                   , '\\bq-tagging: light flavor' ),
             ('pu'                            , 'Pileup reweighting'         ),
             ('scale_j'                       , 'Jet energy scale'           ),
             ('znunu_rzunc'                   , '$R_{\\cPZ}$'                ),
             ('zfromgamma_mcstat_bin'         , 'Monte Carlo statistics (SR)'),
             ('zfromgamma_mcstat_bin_photoncr', 'Monte Carlo statistics (CR)'),
             ('zfromgamma_stat_bin_photoncr'  , 'Data statistics (CR)'       ),
  ],
  'ttz' : [('corr_l'        , 'Lepton veto'               ),
           ('eff_b_heavy'   , '\\bq-tagging: heavy flavor'),
           ('eff_b_light'   , '\\bq-tagging: light flavor'),
           ('lumi'          , 'Luminosity'                ),
           ('pu'            , 'Pileup reweighting'        ),
           ('scale_j'       , 'Jet energy scale'          ),
           ('ttZNorm'       , 'Cross section'             ),
           ('ttZ_pdfunc'    , 'pdf/$\\alpha_S$ variation' ),
           ('ttZ_scaleunc'  , '$\\mu_R/\\mu_F$ variation' ),
           ('mcstat_ttZ_bin', 'Monte Carlo statistics'    ),
  ],
  'qcd' : [('corr_tau'          , 'Tau veto'                  ),
           ('eff_b_heavy'       , '\\bq-tagging: heavy flavor'),
           ('eff_b_light'       , '\\bq-tagging: light flavor'),
           ('pu'                , 'Pileup reweighting'        ),
           ('scale_j'           , 'Jet energy scale'          ),
           ('qcd_bkgsubunc_'    , 'Background subtraction'    ),
           ('qcd_jetresptailunc', 'Jet response tail'         ),
           ('qcd_tfstatunc'     , 'Transfer factor'           ),
           ('qcd_stat_bin_qcdcr', 'Data statistics (SR)'      ),
  ]
} # sources

def main() :
  args = sys.argv[1:]
  if not args:
    print('usage: makeUncTables.py <directory with template datacards>')
    sys.exit(1)
  inDir = args[0]
  
  # LLB 1LCR table
  print '\n\n\n', '='*5, 'Making onelepcr unc table...', '\n\n'
  print makeTable(inDir,'onelepcr','onelepcr')
  
  # tables for the various bkgs
  for k in ('ttbarplusw', 'znunu', 'qcd', 'ttz') : 
    print '\n\n\n', '='*5, 'Making', k, 'unc table...', '\n\n'
    print makeTable(inDir,k)
  print '\n\n\n'

# piece together chunks for the given bkg/cr
def makeTable(inDir,bkg,cr='') :
  s  = makeChunk(inDir,bkg,5,  0,0,cr)
  s += makeChunk(inDir,bkg,7,  0,0,cr)
  s += makeChunk(inDir,bkg,5,175,0,cr)
  s += makeChunk(inDir,bkg,7,175,0,cr)
  s += makeChunk(inDir,bkg,5,175,1,cr)
  s += '\\hline\n'
  return s

# table chunk for a given bin in (njets, mtb, ntops)
def makeChunk(inDir,bkg,nj, mtb, nt,cr='') :
  columns = 11 if cr=='' else 6
  s = chunkHeaderNoB(nj, mtb, nt, columns)
  for source in sources[bkg] :
    s += source[1]
    if cr=='onelepcr' and nt==1 : # combine met bins for nT=1 for the onelepcr
      s += ' & \\multicolumn{5}{c|}{' + getUnc(inDir,source[0],bkg,binsMet[0],nj,1,nt,mtb,cr) + '}  \\\\ \n'
      continue
    for nb in binsB :
      if cr != '' and nb>1 : continue
      for met in binsMet:
        s += ' & ' + getUnc(inDir,source[0],bkg,met,nj,nb,nt,mtb,cr)
    s += ' \\\\ \n'
  return s

# returns the datacard filename for the given bin
#   - use 'sig' to get the datacard for a signal region. if none is given, the template file will be used
#   - use 'cr' to get the file for the CR
def getFileName(inDir,met,nj,nb,nt,mtb,cr='',sig='') :
  fname = 'met'+str(met)+'_njets'+str(nj)+'_nbjets'+str(nb)+'_ncttstd'+str(nt)+'_mtcsv12met'+str(mtb)+'.txt'
  if cr=='onelepcr' : fname = 'onelepcr_'+fname
  if 'T2' in sig : 
    fname = sig+'_'+fname
    return os.path.join(inDir,sig,fname)
  fname = 'T2tt_700_1_'+fname
  fname = os.path.join(inDir,'T2tt_700_1',fname)
  return fname

# get and properly format the number for the given bkg, bin
def getUnc(inDir,unc,process,met,nj,nb,nt,mtb,cr='',sig='') :
  if unc == 'lumi' : return '4.6\\%'
  u = getRawUnc(inDir,unc,process,met,nj,nb,nt,mtb,cr)
  if u == '-' : return u
  return str(int(round(u*100,0)))+'\\%'

# gets the unc for a given bin, bkg as a float or returns '-' if it doesn't exist
def getRawUnc(inDir,unc,process,met,nj,nb,nt,mtb,cr='',sig='') :
  if unc == 'lumi' : return 0.046
  fname = getFileName(inDir,met,nj,nb,nt,mtb,cr,sig)
  if not os.path.exists(fname) : return ' '
  f = open(fname)
  ul = ''
  for l in f:
    if unc in l :
      if unc == 'zfromgamma_mcstat_bin' and 'photon' in l: continue
      ul = l.strip()
  f.close()
  if ul == '' : return '-'
  ul = ul.split() 
  u = 0
  if ul[1] == 'lnN':
    u = ul[ps[process]]
    if u == '-' : return u
    u = (float(u)-1)
  elif ul[1] == 'gmN' :
    s = ul[ps[process]+1]
    if s == '-' : return s
    if s == '0' : return 0
    n = float(ul[2])
    u = sqrt(n)/n
  elif ul[1] == 'lnU' :
    if nt==1 : n = getDataYield(inDir,process,250,nj,1,nt,mtb,'onelepcr')
    else     : n = getDataYield(inDir,process,met,nj,1,nt,mtb,'onelepcr')
    if n<1 : n=1
    u = sqrt(n)/n
  else :
    print '!!!!! Unknown unc type:', ul[1]
    print '!!!!! Needs to be added to getRawUnc'
    return 0
  return u

# returns the 'observation' number from the datacard for the given bin
def getDataYield(inDir,process,met,nj,nb,nt,mtb,cr='',sig='') :
  fname = getFileName(inDir,met,nj,nb,nt,mtb,cr,sig)
  if not os.path.exists(fname) : return 0
  f = open(fname)
  n = ''
  for l in f:
    if len(l) < 1 : continue
    if not 'observation' in l : continue
    n = l.split()[1]
  f.close()
  if n=='' : return '???' # this shouldn't happen unless a file doesn't exist or doesn't have an observation number
  return int(n)

# puts together the bin header for bins of nJets, mtb, nTop (no selection on nB)
def chunkHeaderNoB(nj, mtb, nt, columns) :
  s  = '\\hline\n'
  s += '\\multicolumn{'+str(columns)+'}{c}{'
  if  (mtb==  0 and nj==5          ) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$'
  elif(mtb==  0 and nj==7          ) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$'
  elif(mtb==175 and nj==5 and nt==0) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$'
  elif(mtb==175 and nj==7 and nt==0) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$'
  elif(mtb==175 and nj==5 and nt==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$'
  s += '} \\\\ \n'
  s += '\\hline\n' 
  return s

# puts together the bin header for bins of nJets, mtb, nTop, nB
def chunkHeader(nj, nb, mtb, nt, columns) :
  s  = '\\hline\n'
  s += '\\multicolumn{'+str(columns)+'}{c}{'
  if  (mtb==  0 and nj==5           and nb==1) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$, $\\nb = 1$'
  elif(mtb==  0 and nj==7           and nb==1) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$, $\\nb = 1$'
  elif(mtb==175 and nj==5 and nt==0 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$, $\\nb = 1$'
  elif(mtb==175 and nj==7 and nt==0 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$, $\\nb = 1$'
  elif(mtb==175 and nj==5 and nt==1 and nb==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$, $\\nb = 1$'
  if  (mtb==  0 and nj==5           and nb==2) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$, $\\nb \\geq 2$'
  elif(mtb==  0 and nj==7           and nb==2) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$, $\\nb \\geq 2$'
  elif(mtb==175 and nj==5 and nt==0 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$, $\\nb \\geq 2$'
  elif(mtb==175 and nj==7 and nt==0 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$, $\\nb \\geq 2$'
  elif(mtb==175 and nj==5 and nt==1 and nb==2) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$, $\\nb \\geq 2$'
  #nJets $NJETS, \mtb MTB$~\\GeV, $N_\\mathrm{t} NT$
  s += '} \\\\ \n'
  s += '\\hline\n' 
  return s
  
if __name__=='__main__' : main()
