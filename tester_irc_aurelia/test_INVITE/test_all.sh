. ./utils.sh

subtitle "test INVITE"

sh ./test_INVITE/test_err_401.sh
sh ./test_INVITE/test_err_403.sh
# sh ./test_INVITE/test_err_442.sh
sh ./test_INVITE/test_err_443.sh
sh ./test_INVITE/test_err_451.sh
sh ./test_INVITE/test_err_461.sh
#sh ./test_INVITE/test_err_482.sh
#sh ./test_INVITE/test_ok.sh

echo ""