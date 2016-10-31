#!/bin/bash
for file in comp*
do
    mv -i "${file}" "${file/dphitraining/qcdVariables}"
done
