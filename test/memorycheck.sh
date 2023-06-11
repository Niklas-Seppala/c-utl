
if $1
then
    echo "---------------------------------------------------"
    echo "          Memory issues were detected"
    echo "---------------------------------------------------"
    grep "ERROR SUMMARY:" /tmp/ctl-memcheck.log
    exit 1
else
    @echo "---------------------------------------------------"
    @echo "          No memory issues were detected"
    @echo "---------------------------------------------------"
    exit 0
fi
