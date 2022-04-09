#! /usr/bin/env atf-sh

. $(atf_get_srcdir)/test_helper.sh
elfdbg_path=$(which elfdbg)

atf_test_case stripped

stripped_head()
{
    atf_set "descr" "ELF binary test without debug symbols"

}

stripped_body()
{
    generate_bin_nodbg

    [ ! -f "${elfdbg_path}" ] && atf_fail "No elfdbg binary found in '$PATH'"

    out=$(${elfdbg_path} bin)
    expected="NO DEBUG"
    if [ "$out" == "$expected" ]; then
        atf_pass
    else
        atf_fail "Wrong output w/o debug symbols '$out'"
    fi
}

stripped_cleanup()
{
    rm -rf bin bin.c
}

atf_test_case not_stripped

not_stripped_head()
{
    atf_set "descr" "ELF binary test with debug symbols"
}
not_stripped_body()
{
    generate_bin_dbg

    [ ! -f "${elfdbg_path}" ] && atf_fail "No elfdbg binary found in '$PATH'"

    out=$(${elfdbg_path} bin)
    expected="HAS DEBUG"
    if [ "$out" == "$expected" ]; then
        atf_pass
    else
        atf_fail "Wrong output w/ debug symbols '$out'"
    fi
}

not_stripped_cleanup()
{
    rm -rf bin bin.c
}

atf_init_test_cases()
{
    atf_add_test_case stripped
    atf_add_test_case not_stripped
}
