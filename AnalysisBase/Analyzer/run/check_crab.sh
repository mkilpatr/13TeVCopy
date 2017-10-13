#!/bin/bash

mypath=crab_projects
for i in ${mypath}/*; do
	echo $i
	crab status $i
done
