void output_char(long c)
{
    *(long *)(0x100000208) = (long)c;
    return;
}
void output_q(long n)
{
    long num;
    long c;
    long num2;
    long count;
    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }
    num2 = 0;
    count = 0;
output_q_j1:
    c = num - (num / 10)*10;
    num = num / 10;
    num2 = num2 * 10;
    num2 = num2 + c;
    count = count + 1;
    if (num > 0) {
        goto output_q_j1;
    }
    num = 0;
output_q_j2:
    c = num2 - (num2 / 10)*10;
    num2 = num2 / 10;
    c = c + 48;
    output_char((long)c);
    num = num + 1;
    if (num < count) {
        goto output_q_j2;
    }
    return;
}
void output_q_digits(long n, long d)
{
    long num;
    long c;
    long num2;
    long count;
    num = n;
    if (n < 0) {
       num = 0 - n;
       output_char((long)'-');
    }
    num2 = 0;
    count = 0;
    while (num > 0) {
        c = num - (num / 10)*10;
        num = num / 10;
        num2 = num2 * 10;
        num2 = num2 + c;
        count = count + 1;
    }
    num = 0;
    while (num + count < d) {
        output_char('0');
        num = num + 1;
    }
    num = 0;
    while (num < count) {
        c = num2 - (num2 / 10)*10;
        num2 = num2 / 10;
        c = c + 48;
        output_char((long)c);
        num = num + 1;
    }
    return;
}
void output_q_hex(long n)
{
    long num;
    long c;
    long num2;
    long i;
    num = n;
    num2 = n;
    output_char((long)'0');
    output_char((long)'x');
    num2 = 0;
    i = 0;
output_q_hex_j1:
    c = num & 0xF;
    num = num / 16;
    num2 = num2 * 16;
    num2 = num2 + c;
    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j1;
    }
    i = 0;
output_q_hex_j2:
    c = num2 & 0xF;
    num2 = num2 / 16;
    if (c < 10) {
        c = c + 48;
    } else {
        c = c + 55;
    }
    output_char((long)c);
    i = i + 1;
    if (i < 15) {
        goto output_q_hex_j2;
    }
    return;
}
void output_id()
{
    long id;
    id = *(long*)0x100000120;
    output_q_hex(id);
    return;
}
void output_char_str(char* s, long sl)
{
    long n;
    n = 0;
    while (n < sl) {
        output_char((long)s[n]);
        n = n + 1;
    }
    return;
}
void output_str(char* s)
{
    long n;
    n = 0;
output_str_j1:
    if (s[n] != 0) {
        output_char((long)s[n]);
        n = n + 1;
        goto output_str_j1;
    }
    return;
}
char input_char()
{
    long n;
    n = *(long*)0x100000200;
    return (char)n;
}
long input_q()
{
    long c;
    long is_neg;
    long is_checking;
    long n;
    n = 0;
    is_neg = 0;
    is_checking = 1;
    do {
        c = *(long*)0x100000200;
        if (c == (-1)) goto input_q_exit;
        if (c == (long)'-') {
            c = *(long*)0x100000200;
            if (c <= (long)'9') {
                if (c >= (long)'0') {
                    is_neg = 1;
                    is_checking = 0;
                }
            }
        } else if (c <= (long)'9') {
            if (c >= (long)'0') {
                is_checking = 0;
            }
        }
    } while (is_checking == 1);
    is_checking = 1;
    do {
        n = n * 10 + (long)c - (long)'0';
        c = *(long*)0x100000200;
        if (c == (-1)) goto input_q_exit;
        if (c > (long)'9') {
            is_checking = 0;
        }
        if (c < (long)'0') {
            is_checking = 0;
        }
    } while (is_checking == 1);
input_q_exit:
    if (is_neg == 1) {
        n = 0 - n;
    }
    return n;
}
register long reg1;
register long reg2;
register long reg3;
register long reg4;
long get_my_task_id()
{
    return *(long*)((0x100000120));
}
long get_new_task_id()
{
    return *(long*)((0x100000410));
}



void task_id_to_ec_range(long id, long *base, long *len)
{
    *base = id;
    *len = (0x1000);
    return;
}
standalone long *pmem_in;
standalone long x[(10)];
standalone long y[(10)];
standalone long z[(10)];
standalone long w[(10)];
standalone long ci[(10)];
standalone long cx[(2)];
standalone long cy[(2)];
standalone long cz[(2)];
standalone long cw[(2)];
standalone long lcx[((2)+512)*(10)/(10)];
standalone long lcy[((2)+512)*(10)/(10)];
standalone long lcz[((2)+512)*(10)/(10)];
standalone long lcw[((2)+512)*(10)/(10)];
standalone long lcn[((2)+512)*(10)/(10)];
standalone long trigger[((10)/(10)+1)*512];
standalone long iter;
register long reg1;
register long reg2;
register long reg3;
register long reg4;
void start_iteration();
void srand(long seed)
{
    if (seed == 0) {
        seed = 1;
    }
    *((long *)0x300000000) = seed;
    return;
}
long rand()
{
    long r;
    r = *((long *)0x300000000);
    r = r * 0xfef3f6f4f3f2f1;
    *((long *)0x300000000) = r;
    return r;
}
long distance_sq_0(long x1, long y1, long z1, long w1, long x2, long y2, long z2, long w2)
{
    long xd, yd, zd, wd;
    xd = (x1 - x2);
    yd = (y1 - y2);
    zd = (z1 - z2);
    wd = (w1 - w2);
    return xd * xd + yd * yd + zd * zd + wd * wd;
}
void _output_centroids(long with_point_num)
{
    long i;
    long *npt;
    npt = (long *)(0x300000000 + (2) * 8 * 4);
    output_q((2));
    output_char(' ');
    output_char('c');
    output_char('e');
    output_char('n');
    output_char('t');
    output_char('r');
    output_char('o');
    output_char('i');
    output_char('d');
    output_char('s');
    output_char(':');
    output_char((10));
    for(i = 0; i < (2); i = i + 1)
    {
        output_q(cx[i]);
        output_char(' ');
        output_q(cy[i]);
        output_char(' ');
        output_q(cz[i]);
        output_char(' ');
        output_q(cw[i]);
        if (with_point_num == 1) {
            output_char('/');
            output_q(npt[i]);
        }
        output_char((10));
    }
    output_char((10));
    return;
}
void output_centroids_with_point_num()
{
    _output_centroids(1);
    return;
}
void output_centroids()
{
    _output_centroids(0);
    return;
}
void kmeans_finish()
{
    output_centroids();
    return;
}
void on_recluster_finished_0()
{
    long i;
    long *sumx;
    long *sumy;
    long *sumz;
    long *sumw;
    long *npt;
    sumx = (long *)(0x300000000);
    sumy = (long *)(0x300000000 + (2) * 8);
    sumz = (long *)(0x300000000 + (2) * 8 * 2);
    sumw = (long *)(0x300000000 + (2) * 8 * 3);
    npt = (long *)(0x300000000 + (2) * 8 * 4);
    for(i = 0; i < (10)/(10) * 512; i = i + 512)
    {
        if(trigger[i] != 2)
        {
            abort;
        }
    }
    for(i = 0; i < (2); i = i + 1)
    {
        sumx[i] = 0;
        sumy[i] = 0;
        sumz[i] = 0;
        sumw[i] = 0;
        npt[i] = 0;
    }
    for(i = 0; i < (10); i = i + 1)
    {
        long t;
        long xt, yt, zt, wt, xd, yd, zd, wd;
        t = ci[i];
        xt = x[i];
        yt = y[i];
        zt = z[i];
        wt = w[i];
        sumx[t] = sumx[t] + xt;
        sumy[t] = sumy[t] + yt;
        sumz[t] = sumz[t] + zt;
        sumw[t] = sumw[t] + wt;
        npt[t] = npt[t] + 1;
    }
    for(i = 0; i < (2); i = i + 1)
    {
        if (npt[i] != 0) {
            cx[i] = sumx[i] / npt[i];
            cy[i] = sumy[i] / npt[i];
            cz[i] = sumz[i] / npt[i];
            cw[i] = sumw[i] / npt[i];
        }
        else {
            cx[i] = 0x7FFFFFFFFFFFFFF;
            cy[i] = 0x7FFFFFFFFFFFFFF;
            cz[i] = 0x7FFFFFFFFFFFFFF;
            cw[i] = 0x7FFFFFFFFFFFFFF;
        }
    }
    output_char((10));
    output_char('i');
    output_char('t');
    output_char('e');
    output_char('r');
    output_char(':');
    output_q(iter);
    output_char((10));
    iter = iter + 1;
    if (iter < (20)) {
        start_iteration();
    }
    else {
        kmeans_finish();
    }
    *((long *)(0x100000208)) = 'T';
    output_char(':');
    output_q(iter);
    output_char((10));
    commitd;
}
void on_recluster_finished()
{
    long i;
    long j;
    long *tlcx;
    long *tlcy;
    long *tlcz;
    long *tlcw;
    long *tlcn;
    long n_parts;
    long *sumx;
    long *sumy;
    long *sumz;
    long *sumw;
    long *npt;
    for(i = 0; i < (10)/(10) * 512; i = i + 512)
    {
        if(trigger[i] != 2)
        {
            abort;
        }
    }
    trigger[0] = 3;
    sumx = (long *)(0x300000000);
    sumy = (long *)(0x300000000 + (2) * 8);
    sumz = (long *)(0x300000000 + (2) * 8 * 2);
    sumw = (long *)(0x300000000 + (2) * 8 * 3);
    npt = (long *)(0x300000000 + (2) * 8 * 4);
    for(i = 0; i < (2); i = i + 1)
    {
        sumx[i] = 0;
        sumy[i] = 0;
        sumz[i] = 0;
        sumw[i] = 0;
        npt[i] = 0;
    }
    n_parts = (10) / (10);
    for (j = 0; j < n_parts; j = j + 1) {
        tlcx = &lcx[((2)+512)*j];
        tlcy = &lcy[((2)+512)*j];
        tlcz = &lcz[((2)+512)*j];
        tlcw = &lcw[((2)+512)*j];
        tlcn = &lcn[((2)+512)*j];
 for (i = 0; i < (2); i = i + 1)
 {
     sumx[i] = sumx[i] + tlcx[i];
     sumy[i] = sumy[i] + tlcy[i];
     sumz[i] = sumz[i] + tlcz[i];
     sumw[i] = sumw[i] + tlcw[i];
     npt[i] = npt[i] + tlcn[i];
        }
    }
    for(i = 0; i < (2); i = i + 1)
    {
        if (npt[i] != 0) {
            cx[i] = sumx[i] / npt[i];
            cy[i] = sumy[i] / npt[i];
            cz[i] = sumz[i] / npt[i];
            cw[i] = sumw[i] / npt[i];
        }
        else {
            cx[i] = 0x7FFFFFFFFFFFFFF;
            cy[i] = 0x7FFFFFFFFFFFFFF;
            cz[i] = 0x7FFFFFFFFFFFFFF;
            cw[i] = 0x7FFFFFFFFFFFFFF;
        }
    }
    output_char((10));
    output_char('i');
    output_char('t');
    output_char('e');
    output_char('r');
    output_char(':');
    output_q(iter);
    output_char((10));


    iter = iter + 1;

    if (iter < (20)) {
        start_iteration();
    }
    else {
        kmeans_finish();
    }
    commitd;
}
void recluster_runner_0(long start, long end)
{
    long i, j, mini;
    long mind, d;
    long xt, yt, zt, wt;
    for(i = start; i < end; i = i + 1)
    {
        xt = x[i];
        yt = y[i];
        zt = z[i];
        wt = w[i];
        j = 0;
        d = (((xt) - (cx[j])) * ((xt) - (cx[j]))) + (((yt) - (cy[j])) * ((yt) - (cy[j]))) + (((zt) - (cz[j])) * ((zt) - (cz[j]))) + (((wt) - (cw[j])) * ((wt) - (cw[j])));
        mind = d;
        mini = j;
        for(j = 1; j < (2); j = j + 1)
        {
            d = (((xt) - (cx[j])) * ((xt) - (cx[j]))) + (((yt) - (cy[j])) * ((yt) - (cy[j]))) + (((zt) - (cz[j])) * ((zt) - (cz[j]))) + (((wt) - (cw[j])) * ((wt) - (cw[j])));
            if(d < mind)
            {
                mind = d;
                mini = j;
            }
        }
        if(ci[i] != mini)
        {
            ci[i] = mini;
        }
    }
    trigger[start/(10)*512] = 2;
    commitd;
}
void recluster_runner(long start, long end)
{
    long i, j, mini;
    long mind, d;
    long xt, yt, zt, wt;
    long partid;
    long *mylcx;
    long *mylcy;
    long *mylcz;
    long *mylcw;
    long *mylcn;
    partid = start / (10);
    mylcx = &lcx[((2)+512)*partid];
    mylcy = &lcy[((2)+512)*partid];
    mylcz = &lcz[((2)+512)*partid];
    mylcw = &lcw[((2)+512)*partid];
    mylcn = &lcn[((2)+512)*partid];
    for (i = 0; i < (2); i = i + 1) {
        mylcx[i] = 0;
        mylcy[i] = 0;
        mylcz[i] = 0;
        mylcw[i] = 0;
        mylcn[i] = 0;
    }
    for(i = start; i < end; i = i + 1)
    {
        xt = x[i];
        yt = y[i];
        zt = z[i];
        wt = w[i];
        j = 0;
        reg1 = (xt) - (cx[j]) ; reg2 = (yt) - (cy[j]) ; reg3 = (zt) - (cz[j]) ; reg4 = (wt) - (cw[j]) ; *(&d) = reg1*reg1 + reg2*reg2 + reg3*reg3 + reg4*reg4;
        mind = d;
        mini = j;
        for(j = 1; j < (2); j = j + 1)
        {
            reg1 = (xt) - (cx[j]) ; reg2 = (yt) - (cy[j]) ; reg3 = (zt) - (cz[j]) ; reg4 = (wt) - (cw[j]) ; *(&d) = reg1*reg1 + reg2*reg2 + reg3*reg3 + reg4*reg4;
            if(d < mind)
            {
                mind = d;
                mini = j;
            }
        }
        mylcn[mini] = mylcn[mini] + 1;
        mylcx[mini] = mylcx[mini] + xt;
        mylcy[mini] = mylcy[mini] + yt;
        mylcz[mini] = mylcz[mini] + zt;
        mylcw[mini] = mylcw[mini] + wt;
    }
    trigger[start/(10)*512] = 2;
    commitd;
}
void start_iteration()
{
    long i;
    long tmp1;
    long tmp2;
    for(i = 0; i < (10); i = i + (10))
    {
        trigger[i/(10)*512] = 0;
        tmp1 = ((2)+512)*i/(10);
        tmp2 = tmp1 + (2) + 512;
        runner
            recluster_runner(i, i + (10))
            using x[i, , i + (10)], y[i, , i + (10)], z[i, , i + (10)], w[i, , i + (10)], ci[i, , i + (10)], cx[0, , (2)], cy[0, , (2)], cz[0, , (2)], cw[0, , (2)], trigger[i/(10)*512, , (i/(10)+1)*512]
 , lcx[tmp1,, tmp2], lcy[tmp1,, tmp2], lcz[tmp1,, tmp2], lcw[tmp1,, tmp2], lcn[tmp1,, tmp2]
 ;
    }
    runner
        on_recluster_finished()
        using iter, cx[0, , (2)], cy[0, , (2)], cz[0, , (2)], cw[0, , (2)]
        , lcx[0,, ((2)+512)*(10)/(10)], lcy[0,, ((2)+512)*(10)/(10)], lcz[0,, ((2)+512)*(10)/(10)], lcw[0,, ((2)+512)*(10)/(10)], lcn[0,, ((2)+512)*(10)/(10)]
        watching trigger[0, , ((10)/(10)+1)*512]
        ;
    return;
}
void on_init1_finished()
{
    long i;
    for(i = 0; i < (10)/(10) * 512; i = i + 512)
    {
        if(trigger[i] != 1)
        {
            abort;
        }
    }
    trigger[0] = 3;
    for(i = 0; i < (2); i = i + 1)
    {
        cx[i] = x[i];
        cy[i] = y[i];
        cz[i] = z[i];
        cw[i] = w[i];
    }
    iter = 0;
    start_iteration();
    commitd;
}
void init_runner(long start, long end)
{
    long i;
    long t;
    srand(1);
    for(i = 0; i < start; i = i + 1)
    {
     t = input_q();
     t = input_q();
     t = input_q();
     t = input_q();
    }
    for(i = start; i < end; i = i + 1)
    {
        x[i] = input_q();
        y[i] = input_q();
        z[i] = input_q();
        w[i] = input_q();
    }
    trigger[start/(10)*512] = 1;
    commit;
}
void init_runner_stdin(long start, long end)
{
    long i;
    long t;
    for(i = 0; i < start; i = i + 1)
    {
     t = input_q();
     t = input_q();
     t = input_q();
     t = input_q();
    }
    for(i = start; i < end; i = i + 1)
    {
        x[i] = input_q();
        y[i] = input_q();
        z[i] = input_q();
        w[i] = input_q();
    }
    for(i = start; i < end; i = i + 1) {
        trigger[i/(10)*512] = 1;
    }
    commit;
}
void init0()
{
    long i;
    for(i = 0; i < (10); i = i + (10))
    {
        trigger[i/(10)*512] = 0;
        runner init_runner(i, i + (10))
            using x[i, , i + (10)], y[i, , i + (10)], z[i, , i + (10)], w[i, , i + (10)], trigger[i/(10)*512, , (i/(10)+1)*512]
 ;
    }
    runner on_init1_finished()
        using cx[0, , (2)], cy[0, , (2)], cz[0, , (2)], cw[0, , (2)], x[0, , (2)], y[0, , (2)], z[0, , (2)], w[0, , (2)], trigger[0, , (10)], iter
        watching trigger[0, , ((10)/(10)+1)*512];
    commit;
}
void init_runner_pmem(long start, long end)
{
    long i;
    long t;
    for(i = start; i < end; i = i + 1)
    {
        x[i] = pmem_in[4*i];
        y[i] = pmem_in[4*i+1];
        z[i] = pmem_in[4*i+2];
        w[i] = pmem_in[4*i+3];
    }
    trigger[start/(10)*512] = 1;
    commit;
}
void init0_in_pmem()
{
    long i;
    pmem_in = (long*)0x40000000000;
    for(i = 0; i < (10); i = i + (10))
    {
        trigger[i/(10)*512] = 0;
        runner init_runner_pmem(i, i + (10))
            using x[i, , i + (10)], y[i, , i + (10)], z[i, , i + (10)], w[i, , i + (10)], trigger[i/(10)*512, , (i/(10)+1)*512]
            , pmem_in
            , pmem_in[4*i,, 4*(i + (10))]
            ;
    }
    runner on_init1_finished()
        using cx[0, , (2)], cy[0, , (2)], cz[0, , (2)], cw[0, , (2)], x[0, , (2)], y[0, , (2)], z[0, , (2)], w[0, , (2)], trigger[0, , (10)], iter
        , pmem_in
        watching trigger[0, , ((10)/(10)+1)*512];
    commit;
}
void main()
{
    runner init0_in_pmem() using trigger[0, , ((10)/(10)+1)*512],
           pmem_in;
    commit;
}
