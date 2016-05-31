
cat > submit.cmd <<EOF
Requirements            = (Arch == "X86_64") && (OpSys == "LINUX")
request_disk            = 10000000
request_memory          = 10000
Executable              = runjobscondor.sh
Arguments               = MACRO_NAME INFILE_PATH_NAME INDEX TREE_NAME SUFFIX SMEAR_TEMPLATE_NAME IS_MC OUTPUT_DIR SCRAM_DIR OUTFILE_NAME OUTFILE_DIR PREFIX
Output                  = logs/INFILE_NAME.out
Error                   = logs/INFILE_NAME.err
Log                     = logs/INFILE_NAME.log
use_x509userproxy       = true
initialdir              = OUTPUT_DIR
Should_Transfer_Files   = YES
transfer_input_files    = ${CMSSW_BASE}/MACRO_NAME,${CMSSW_BASE}/rootlogon.C
Queue
EOF

  condor_submit submit.cmd;
  rm submit.cmd

