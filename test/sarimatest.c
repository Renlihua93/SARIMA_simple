// SPDX-License-Identifier: BSD-3-Clause
/*
 * sarimatest.c - SARIMA 示例程序
 *
 * 功能: 读取 seriesG.txt, 对数变换, 拟合季节 ARIMA, 打印摘要与预测。
 * 须用 CMake 目标 sarimatest 链接 sarimalib 构建。
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sarima.h"

#ifndef CTSA_DATA_FILE
#error "CTSA_DATA_FILE must be set by CMake."
#endif






/*
 * main - 程序入口: 加载数据、拟合 SARIMA、输出预测
 */
int main(void)
{
	const int p = 0, d = 1, q = 1, s = 12, P = 0, D = 1, Q = 1, L = 5;
	double temp[1200];
	double *inp, *xpred, *amse;
	sarima_object model;
	FILE *fp;
	int i, N;

	xpred = (double *)malloc((size_t)L * sizeof(double));
	amse = (double *)malloc((size_t)L * sizeof(double));
	if (!xpred || !amse)
		return 1;

	fp = fopen(CTSA_DATA_FILE, "r");
	if (!fp) {
		fprintf(stderr, "Cannot open %s\n", CTSA_DATA_FILE);
		return 100;
	}
	for (i = 0; !feof(fp) && i < 1200; ++i)
		fscanf(fp, "%lf \n", &temp[i]);
	fclose(fp);
	N = i;

	inp = (double *)malloc((size_t)N * sizeof(double));
	for (i = 0; i < N; ++i)
		inp[i] = log(temp[i]);

	model = sarima_init(p, d, q, s, P, D, Q, N);
	sarima_setMethod(model, 0);
	sarima_exec(model, inp);
	sarima_summary(model);
	sarima_predict(model, inp, L, xpred, amse);

	printf("\nPredicted Values : ");
	for (i = 0; i < L; ++i)
		printf("%g ", xpred[i]);
	printf("\nStandard Errors  : ");
	for (i = 0; i < L; ++i)
		printf("%g ", sqrt(amse[i]));
	printf("\n");

	sarima_free(model);
	free(inp);
	free(xpred);
	free(amse);
	return 0;
}
