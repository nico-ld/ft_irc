. ./utils.sh

subtitle "test KICK"

sh ./test_KICK/test_err_401.sh
sh ./test_KICK/test_err_403.sh
# sh ./test_KICK/test_err_442.sh
sh ./test_KICK/test_err_451.sh
sh ./test_KICK/test_err_461.sh
sh ./test_KICK/test_ok.sh

#sh test_err_482.sh a faire en manuel

#err_401 err_451

echo ""
