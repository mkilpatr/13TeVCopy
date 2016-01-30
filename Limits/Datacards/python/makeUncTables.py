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
  
  print '\n\n\n', '='*5, 'Making onelepcr unc table...', '\n\n'
  print makeTable(inDir,'onelepcr','onelepcr')
  for k in ['ttbarplusw', 'znunu', 'qcd', 'ttz'] : 
    print '\n\n\n', '='*5, 'Making', k, 'unc table...', '\n\n'
    print makeTable(inDir,k)
  print '\n\n\n'

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
           ('ttZ_pdfunc'    , 'pdf/$\\alpha_S$ variation' ),
           ('ttZ_scaleunc'  , '$\\mu_R/\\mu_F$ variation' ),
           ('ttZNorm'       , 'Cross section'             ),
           ('mcstat_ttZ_bin', 'Monte Carlo statistics'    ),
  ],
  'qcd' : [('corr_tau'          , 'Tau veto'                  ),
           ('eff_b_heavy'       , '\\bq-tagging: heavy flavor'),
           ('eff_b_light'       , '\\bq-tagging: light flavor'),
           ('pu'                , 'Pileup reweighting'        ),
           ('scale_j'           , 'Jet energy scale'          ),
           ('qcd_jetresptailunc', 'Jet response tail'         ),
           ('qcd_bkgsubunc_'    , 'Background subtraction'    ),
           ('qcd_tfstatunc_bin' , 'Transfer factor'           ),
           ('qcd_stat_bin_qcdcr', 'Data statistics (SR)'      ),
  ]
}

def makeTable(inDir,bkg,cr=''):
  s  = makeChunk(inDir,bkg,5,  0,0,cr)
  s += makeChunk(inDir,bkg,7,  0,0,cr)
  s += makeChunk(inDir,bkg,5,175,0,cr)
  s += makeChunk(inDir,bkg,7,175,0,cr)
  s += makeChunk(inDir,bkg,5,175,1,cr)
  s += '\\hline\n'
  return s

def makeChunk(inDir,bkg,nj, mtb, nt,cr='') :
  s = chunkHeader(nj, mtb, nt)
  if cr != '' : s = s.replace('multicolumn{11}','multicolumn{6}')
  for source in sources[bkg] :
    s += source[1]
    if cr=='onelepcr' and nj==5 and mtb==175 and nt==1 : 
      # combine met bins for nT=1 for the onelepcr
      s += ' & \\multicolumn{5}{c|}{' + getUnc(inDir,source[0],bkg,binsMet[0],nj,1,nt,mtb,cr) + '}  \\\\ \n'
      continue
    for nb in binsB :
      if cr != '' and nb>1 : continue
      for met in binsMet:
        s += ' & ' + getUnc(inDir,source[0],bkg,met,nj,nb,nt,mtb,cr)
    s += ' \\\\ \n'
  return s

def getUnc(inDir,unc,process,met,nj,nb,nt,mtb,cr=''):
  if unc == 'lumi' : return '4.6\\%'
  fname = 'met'+str(met)+'_njets'+str(nj)+'_nbjets'+str(nb)+'_ncttstd'+str(nt)+'_mtcsv12met'+str(mtb)+'.txt'
  if cr=='' : fname = fname+'_sr'
  if cr!='' : fname = cr+'_'+fname+'_'+cr
  fname = 'template_'+fname+'_template'
  fname = os.path.join(inDir,fname)
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
    if s == '0' : return '0\\%'
    n = float(ul[2])
    u = sqrt(n)/n
  u *= 100
  if u>0 : u += 0.5
  else   : u -= 0.5
  return str(int(u))+'\\%'

def chunkHeader(nj, mtb, nt) :
  s  = '\\hline\n'
  s += '\\multicolumn{11}{c}{'
  if  (mtb==  0 and nj==5          ) : s += '$\\mtb < 175$~\\GeV, $5\\leq\\nj<7$'
  elif(mtb==  0 and nj==7          ) : s += '$\\mtb < 175$~\\GeV, $\\nj\\geq7$'
  elif(mtb==175 and nj==5 and nt==0) : s += '$\\mtb \\geq 175$~\\GeV, $5\\leq\\nj<7$, $N_\\mathrm{t} = 0$'
  elif(mtb==175 and nj==7 and nt==0) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq7$ $N_\\mathrm{t} = 0$'
  elif(mtb==175 and nj==5 and nt==1) : s += '$\\mtb \\geq 175$~\\GeV, $\\nj\\geq5$, $N_\\mathrm{t} \\geq 1$'
  #nJets $NJETS, \mtb MTB$~\\GeV, $N_\\mathrm{t} NT$
  s += '} \\\\ \n'
  s += '\\hline\n' 
  return s
  
if __name__=='__main__': main()
