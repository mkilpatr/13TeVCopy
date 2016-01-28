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
  
  for k in sorted(ps.keys()) : 
    print '\n\n\n', '='*5, 'Making', k, 'unc table...', '\n\n'
    print makeTable(inDir,k)
  print '\n\n\n'

binsB   = (1,2)
binsMet = (250,300,400,500,600)
ps = {'ttbarplusw':3, 'znunu':4, 'ttz':5, 'qcd':6}
sources = {
  'ttbarplusw' : [('corr_l'               , 'Lepton veto'                ),
                  ('corr_tau'             , 'Tau veto'                   ),
                  ('eff_b_heavy'          , '\cPqb-tagging: heavy flavor'),
                  ('eff_b_light'          , '\cPqb-tagging: light flavor'),
                  ('pu'                   , 'Pileup reweighting'         ),
                  ('scale_j'              , 'Jet energy scale'           ),
                  ('ttbarNorm'            , '\\ttbar normalization'      ),
                  ('wjetsNorm'            , '\\W+jets normalization'     ),
                  ('lostlep_bin_onelepcr' , 'Data statistics (CR)'       ),
                  ('mcstat_ttbarplusw_bin', 'Monte Carlo statistics (SR)'),
  ],
  'znunu' : [('corr_l'                        , 'Lepton veto'                ),
             ('eff_b_heavy'                   , '\cPqb-tagging: heavy flavor'),
             ('eff_b_light'                   , '\cPqb-tagging: light flavor'),
             ('pu'                            , 'Pileup reweighting'         ),
             ('scale_j'                       , 'Jet energy scale'           ),
             ('znunu_rzunc'                   , '$R_{\\cPZ}$'                ),
             ('zfromgamma_stat_bin_photoncr'  , 'Data statistics (CR)'       ),
             ('zfromgamma_mcstat_bin'         , 'Monte Carlo statistics (SR)'),
             ('zfromgamma_mcstat_bin_photoncr', 'Monte Carlo statistics (CR)'),
  ],
  'ttz' : [('corr_l'        , 'Lepton veto'                ),
           ('eff_b_heavy'   , '\cPqb-tagging: heavy flavor'),
           ('eff_b_light'   , '\cPqb-tagging: light flavor'),
           ('lumi'          , 'Luminosity'                 ),
           ('pu'            , 'Pileup reweighting'         ),
           ('scale_j'       , 'Jet energy scale'           ),
           ('ttZ_pdfunc'    , 'Pdf uncertanty'             ),
           ('ttZ_scaleunc'  , '\\ttZ~scale'                ),
           ('ttZNorm'       , '\\ttZ~normalization'        ),
           ('mcstat_ttZ_bin', 'Monte Carlo statistics (SR)'),
  ],
  'qcd' : [('corr_tau'          , 'Tau veto'                   ),
           ('eff_b_heavy'       , '\cPqb-tagging: heavy flavor'),
           ('eff_b_light'       , '\cPqb-tagging: light flavor'),
           ('pu'                , 'Pileup reweighting'         ),
           ('qcd_jetresptailunc', 'Jet response tail'          ),
           ('qcd_stat_bin_qcdcr', 'Monte Carlo statistics (SR)'),
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
    u = sqrt(n)/n
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
