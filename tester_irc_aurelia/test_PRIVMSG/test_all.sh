. ./utils.sh

subtitle "test PRIVMSG"

sh ./test_PRIVMSG/test_err_401.sh
sh ./test_PRIVMSG/test_err_403.sh
sh ./test_PRIVMSG/test_err_412.sh
# sh ./test_PRIVMSG/test_err_442.sh
sh ./test_PRIVMSG/test_err_451.sh
sh ./test_PRIVMSG/test_err_461.sh
sh ./test_PRIVMSG/test_ok.sh

echo ""
