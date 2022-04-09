generate_bin_nodbg()
{
	cat > bin.c <<EOF
int
main(void) {
    return (0);
}
EOF
	/usr/bin/cc -o bin bin.c
	/usr/bin/strip bin
}

generate_bin_dbg()
{
	cat > bin.c <<EOF
int
main(void) {
    return (0);
}
EOF
	/usr/bin/cc -g -o bin bin.c
}

