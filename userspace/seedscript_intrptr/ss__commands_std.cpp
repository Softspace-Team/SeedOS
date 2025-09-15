#include "./ss__commands.hpp"
#include <fstream>
#include <algorithm>

using namespace SeedScript_intrptr;

static CmdResult cmd_echo(CmdCall& c, Context&) {
    CmdResult r; r.out = c.out; r.err = c.err;
    for (size_t i=0;i<c.args.size();++i) {
        if (i) r.out->buf += " ";
        r.out->buf += c.args[i];
    }
    r.out->buf += "\n";
    return r;
}

static CmdResult cmd_upper(CmdCall& c, Context&) {
    CmdResult r; r.out = c.out; r.err = c.err;
    if (c.in) r.out->buf = c.in->buf;
    std::transform(r.out->buf.begin(), r.out->buf.end(), r.out->buf.begin(), ::toupper);
    return r;
}

static CmdResult cmd_read(CmdCall& c, Context&) {
    CmdResult r; r.out = c.out; r.err = c.err;
    if (c.args.empty()) { r.status=2; if(r.err) r.err->buf+="read: missing file\n"; return r; }
    std::ifstream f(c.args[0], std::ios::binary);
    if (!f) { r.status=1; if(r.err) r.err->buf+="read: open fail\n"; return r; }
    r.out->buf.assign(std::istreambuf_iterator<char>(f), {});
    return r;
}

static CmdResult cmd_write(CmdCall& c, Context&) {
    CmdResult r; r.out = c.out; r.err = c.err;
    if (c.args.empty()) { r.status=2; if(r.err) r.err->buf+="write: missing file\n"; return r; }
    std::ofstream f(c.args[0], std::ios::binary | std::ios::app);
    if (!f) { r.status=1; if(r.err) r.err->buf+="write: open fail\n"; return r; }
    if (c.in) f << c.in->buf;
    return r;
}

void SeedScript_intrptr::install_std(CmdRegistry& R) {
    R.add("echo",  cmd_echo);
    R.add("upper", cmd_upper);
    R.add("read",  cmd_read);
    R.add("write", cmd_write);
}