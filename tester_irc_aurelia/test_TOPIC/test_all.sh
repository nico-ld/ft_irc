. ./utils.sh

subtitle "test TOPIC"

sh ./test_TOPIC/test_cmd_TOPIC.sh
sh ./test_TOPIC/test_err_403.sh
sh ./test_TOPIC/test_err_451.sh
sh ./test_TOPIC/test_err_461.sh
sh ./test_TOPIC/test_err_482.sh
sh ./test_TOPIC/test_rpl_331.sh
sh ./test_TOPIC/test_rpl_332.sh

echo ""
