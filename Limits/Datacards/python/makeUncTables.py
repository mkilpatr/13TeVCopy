#! /usr/bin/python
import os
from math import sqrt
import sys

"""Makes the uncertanty tables for the AN for the various backgrounds using the template datacards. If the
uncertanty names change, the "sources" dictornary will need to be updated apropriately. Other backgrounds
can be added as new lists, just make sure that the numbers assigned to each bkg in the "ps" dictionary
is correct. 
"""

def main():
  args = sys.argv[1:]
  if not args:
    print('usage: makeUncTables.py <directory with template datacards>')
    sys.exit(1)
  inDir = args[0]
  
  print '\n\n\n', '='*5, 'Making LLB unc table...', '\n\n'
  print makeTable(inDir,'ttbarplusw')
  print '\n\n\n', '='*5, 'Making Znunu unc table...', '\n\n'
  print makeTable(inDir,'znunu')
  print '\n\n\n', '='*5, 'Making ttZ unc table...', '\n\n'
  print makeTable(inDir,'ttz')
  print '\n\n\n'

binsB   = (1,2)
binsMet = (250,300,400,500,600)
ps = {'signal':2, 'ttbarplusw':3, 'znunu':4, 'ttz':5}
sources = {
  "ttbarplusw" : [('corr_l'               , 'Lepton/tau veto'        ),
                  ('eff_b_heavy'          , 'B-tagging: heavy flavor'),
                  ('eff_b_light'          , 'b-tagging: light flavor'),
                  ('lostlep_bin_onelepcr' , 'data statistics (CR)'   ),
                  ('mcstat_ttbarplusw_bin', 'Monte Carlo statistics' ),
                  ('pu'                   , 'Pileup reweighting'     ),
                  ('scale_j'              , 'Jet energy scale'       ),
                  ('ttbarNorm'            , '\\ttbar normalization'  ),
                  ('wjetsNorm'            , '\\W+jets normalization' ),
  ],
  "znunu" : [('corr_l'                        , 'Lepton/tau veto'                     ),
             ('eff_b_heavy'                   , 'B-tagging: heavy flavor'             ),
             ('eff_b_light'                   , 'b-tagging: light flavor'             ),
             ('pu'                            , 'Pileup reweighting'                  ),
             ('scale_j'                       , 'Jet energy scale'                    ),
             ('zfromgamma_mcstat_bin'         , 'Monte Carlo statistics (SR)'         ),
             ('zfromgamma_mcstat_bin_photoncr', 'Monte Carlo statistics (CR)'         ),
             ('zfromgamma_stat_bin_photoncr'  , 'data statistics (CR)'                ),
             ('znunu_rzunc_1b'                , '$R_{\\cPZ}$ \\met extrapolation (1B)'),
             ('znunu_rzunc_2b'                , '$R_{\\cPZ}$ \\met extrapolation (2B)'),
  ],
  "ttz" : [('corr_l'        , 'Lepton/tau veto'        ),
           ('eff_b_heavy'   , 'B-tagging: heavy flavor'),
           ('eff_b_light'   , 'b-tagging: light flavor'),
           ('lumi'          , 'Luminosity'             ),
           ('mcstat_ttZ_bin', 'Monte Carlo statistics' ),
           ('pu'            , 'Pileup reweighting'     ),
           ('scale_j'       , 'Jet energy scale'       ),
           ('ttZ_pdfunc'    , 'pdf uncertanty'         ),
           ('ttZ_scaleunc'  , '\\ttZ~scale'            ),
           ('ttZNorm'       , '\\ttZ~normalization'    ),
  ] 
}

def makeTable(inDir,bkg):
  s  = makeChunk(inDir,bkg,5,  0,0)
  s += makeChunk(inDir,bkg,7,  0,0)
  s += makeChunk(inDir,bkg,5,175,0)
  s += makeChunk(inDir,bkg,7,175,0)
  s += makeChunk(inDir,bkg,5,175,1)
  s += '\\hline\n'
  return s

def makeChunk(inDir,bkg,nj, mtb, nt) :
  s = chunkHeader(nj, mtb, nt)
  for source in sources[bkg] :
    s += source[1]
    for nb in binsB :
      for met in binsMet:
        s += ' & ' + getUnc(inDir,source[0],bkg,met,nj,nb,nt,mtb)
    s += ' \\\\ \n'
  return s

def getUnc(inDir,unc,process,met,nj,nb,nt,mtb,cr=''):
  fname = 'met'+str(met)+'_njets'+str(nj)+'_nbjets'+str(nb)+'_ncttstd'+str(nt)+'_mtcsv12met'+str(mtb)+'.txt'
  if cr=='' : fname = fname+'_sr'
  if cr!='' : fname = cr+'_'+fname+'_'+cr
  fname = 'template_'+fname+'_template'
  f = open(os.path.join(inDir,fname))
  ul = ''
  for l in f:
    if unc in l :
      ul = l.strip()
      continue 
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
    if s == '0' : return '0\\%'
    n = float(ul[2])
    u = sqrt(n*float(s))/n
  u *= 100
  if u>0 : u += 0.5
  else   : u -= 0.5
  return str(int(u))+'\\%'

def chunkHeader(nj, mtb, nt) :
  if nj==5 : nj='5-6'
  if nj==7 : nj='\\geq 7'
  if mtb==0   : mtb='< 175'
  if mtb==175 : mtb='\\geq 175'
  if nt==0 : nt='= 0'
  if nt==1 : nt='\\geq 1'
  s  = '\\hline\n'
  s += '\\multicolumn{11}{c}{nJets $NJETS, \mtb MTB$~\\GeV, $N_\\mathrm{t} NT$} \\\\ \n'
  s += '\\hline\n' 
  s = s.replace('NJETS',nj )
  s = s.replace('MTB'  ,mtb)
  s = s.replace('NT'   ,nt )
  return s
  
if __name__=='__main__': main()
