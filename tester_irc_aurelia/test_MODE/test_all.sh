. ./utils.sh

subtitle "test MODE"

sh ./test_MODE/test_err_401.sh
sh ./test_MODE/test_err_403.sh
sh ./test_MODE/test_err_461.sh
# sh ./test_MODE/test_err_467.sh
sh ./test_MODE/test_err_472.sh
sh ./test_MODE/test_err_482.sh
sh ./test_MODE/test_ok.sh

#sh test_err_442.sh a faire en manuel

echo ""