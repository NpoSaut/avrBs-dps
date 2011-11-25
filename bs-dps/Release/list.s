GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 1


   1               		.file	"main.cpp"
   2               	__SREG__ = 0x3f
   3               	__SP_H__ = 0x3e
   4               	__SP_L__ = 0x3d
   5               	__CCP__  = 0x34
   6               	__tmp_reg__ = 0
   7               	__zero_reg__ = 1
   8               		.section	.text.__vector_1,"ax",@progbits
   9               	.global	__vector_1
  10               		.type	__vector_1, @function
  11               	__vector_1:
  12 0000 1F92      		push __zero_reg__
  13 0002 0F92      		push r0
  14 0004 0FB6      		in r0,__SREG__
  15 0006 0F92      		push r0
  16 0008 0BB6      		in r0,91-32
  17 000a 0F92      		push r0
  18 000c 1124      		clr __zero_reg__
  19 000e 2F93      		push r18
  20 0010 3F93      		push r19
  21 0012 4F93      		push r20
  22 0014 5F93      		push r21
  23 0016 6F93      		push r22
  24 0018 7F93      		push r23
  25 001a 8F93      		push r24
  26 001c 9F93      		push r25
  27 001e AF93      		push r26
  28 0020 BF93      		push r27
  29 0022 EF93      		push r30
  30 0024 FF93      		push r31
  31               	/* prologue: Signal */
  32               	/* frame size = 0 */
  33 0026 E091 0000 		lds r30,INT0_handler+2
  34 002a F091 0000 		lds r31,(INT0_handler+2)+1
  35 002e 8091 0000 		lds r24,INT0_handler
  36 0032 9091 0000 		lds r25,(INT0_handler)+1
  37 0036 0995      		icall
  38               	/* epilogue start */
  39 0038 FF91      		pop r31
  40 003a EF91      		pop r30
  41 003c BF91      		pop r27
  42 003e AF91      		pop r26
  43 0040 9F91      		pop r25
  44 0042 8F91      		pop r24
  45 0044 7F91      		pop r23
  46 0046 6F91      		pop r22
  47 0048 5F91      		pop r21
  48 004a 4F91      		pop r20
  49 004c 3F91      		pop r19
  50 004e 2F91      		pop r18
  51 0050 0F90      		pop r0
  52 0052 0BBE      		out 91-32,r0
  53 0054 0F90      		pop r0
  54 0056 0FBE      		out __SREG__,r0
  55 0058 0F90      		pop r0
  56 005a 1F90      		pop __zero_reg__
  57 005c 1895      		reti
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 2


  58               		.size	__vector_1, .-__vector_1
  59               		.section	.text.__vector_2,"ax",@progbits
  60               	.global	__vector_2
  61               		.type	__vector_2, @function
  62               	__vector_2:
  63 0000 1F92      		push __zero_reg__
  64 0002 0F92      		push r0
  65 0004 0FB6      		in r0,__SREG__
  66 0006 0F92      		push r0
  67 0008 0BB6      		in r0,91-32
  68 000a 0F92      		push r0
  69 000c 1124      		clr __zero_reg__
  70 000e 2F93      		push r18
  71 0010 3F93      		push r19
  72 0012 4F93      		push r20
  73 0014 5F93      		push r21
  74 0016 6F93      		push r22
  75 0018 7F93      		push r23
  76 001a 8F93      		push r24
  77 001c 9F93      		push r25
  78 001e AF93      		push r26
  79 0020 BF93      		push r27
  80 0022 EF93      		push r30
  81 0024 FF93      		push r31
  82               	/* prologue: Signal */
  83               	/* frame size = 0 */
  84 0026 E091 0000 		lds r30,INT1_handler+2
  85 002a F091 0000 		lds r31,(INT1_handler+2)+1
  86 002e 8091 0000 		lds r24,INT1_handler
  87 0032 9091 0000 		lds r25,(INT1_handler)+1
  88 0036 0995      		icall
  89               	/* epilogue start */
  90 0038 FF91      		pop r31
  91 003a EF91      		pop r30
  92 003c BF91      		pop r27
  93 003e AF91      		pop r26
  94 0040 9F91      		pop r25
  95 0042 8F91      		pop r24
  96 0044 7F91      		pop r23
  97 0046 6F91      		pop r22
  98 0048 5F91      		pop r21
  99 004a 4F91      		pop r20
 100 004c 3F91      		pop r19
 101 004e 2F91      		pop r18
 102 0050 0F90      		pop r0
 103 0052 0BBE      		out 91-32,r0
 104 0054 0F90      		pop r0
 105 0056 0FBE      		out __SREG__,r0
 106 0058 0F90      		pop r0
 107 005a 1F90      		pop __zero_reg__
 108 005c 1895      		reti
 109               		.size	__vector_2, .-__vector_2
 110               		.section	.text.__vector_3,"ax",@progbits
 111               	.global	__vector_3
 112               		.type	__vector_3, @function
 113               	__vector_3:
 114 0000 1F92      		push __zero_reg__
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 3


 115 0002 0F92      		push r0
 116 0004 0FB6      		in r0,__SREG__
 117 0006 0F92      		push r0
 118 0008 0BB6      		in r0,91-32
 119 000a 0F92      		push r0
 120 000c 1124      		clr __zero_reg__
 121 000e 2F93      		push r18
 122 0010 3F93      		push r19
 123 0012 4F93      		push r20
 124 0014 5F93      		push r21
 125 0016 6F93      		push r22
 126 0018 7F93      		push r23
 127 001a 8F93      		push r24
 128 001c 9F93      		push r25
 129 001e AF93      		push r26
 130 0020 BF93      		push r27
 131 0022 EF93      		push r30
 132 0024 FF93      		push r31
 133               	/* prologue: Signal */
 134               	/* frame size = 0 */
 135 0026 E091 0000 		lds r30,INT2_handler+2
 136 002a F091 0000 		lds r31,(INT2_handler+2)+1
 137 002e 8091 0000 		lds r24,INT2_handler
 138 0032 9091 0000 		lds r25,(INT2_handler)+1
 139 0036 0995      		icall
 140               	/* epilogue start */
 141 0038 FF91      		pop r31
 142 003a EF91      		pop r30
 143 003c BF91      		pop r27
 144 003e AF91      		pop r26
 145 0040 9F91      		pop r25
 146 0042 8F91      		pop r24
 147 0044 7F91      		pop r23
 148 0046 6F91      		pop r22
 149 0048 5F91      		pop r21
 150 004a 4F91      		pop r20
 151 004c 3F91      		pop r19
 152 004e 2F91      		pop r18
 153 0050 0F90      		pop r0
 154 0052 0BBE      		out 91-32,r0
 155 0054 0F90      		pop r0
 156 0056 0FBE      		out __SREG__,r0
 157 0058 0F90      		pop r0
 158 005a 1F90      		pop __zero_reg__
 159 005c 1895      		reti
 160               		.size	__vector_3, .-__vector_3
 161               		.section	.text.__vector_4,"ax",@progbits
 162               	.global	__vector_4
 163               		.type	__vector_4, @function
 164               	__vector_4:
 165 0000 1F92      		push __zero_reg__
 166 0002 0F92      		push r0
 167 0004 0FB6      		in r0,__SREG__
 168 0006 0F92      		push r0
 169 0008 0BB6      		in r0,91-32
 170 000a 0F92      		push r0
 171 000c 1124      		clr __zero_reg__
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 4


 172 000e 2F93      		push r18
 173 0010 3F93      		push r19
 174 0012 4F93      		push r20
 175 0014 5F93      		push r21
 176 0016 6F93      		push r22
 177 0018 7F93      		push r23
 178 001a 8F93      		push r24
 179 001c 9F93      		push r25
 180 001e AF93      		push r26
 181 0020 BF93      		push r27
 182 0022 EF93      		push r30
 183 0024 FF93      		push r31
 184               	/* prologue: Signal */
 185               	/* frame size = 0 */
 186 0026 E091 0000 		lds r30,INT3_handler+2
 187 002a F091 0000 		lds r31,(INT3_handler+2)+1
 188 002e 8091 0000 		lds r24,INT3_handler
 189 0032 9091 0000 		lds r25,(INT3_handler)+1
 190 0036 0995      		icall
 191               	/* epilogue start */
 192 0038 FF91      		pop r31
 193 003a EF91      		pop r30
 194 003c BF91      		pop r27
 195 003e AF91      		pop r26
 196 0040 9F91      		pop r25
 197 0042 8F91      		pop r24
 198 0044 7F91      		pop r23
 199 0046 6F91      		pop r22
 200 0048 5F91      		pop r21
 201 004a 4F91      		pop r20
 202 004c 3F91      		pop r19
 203 004e 2F91      		pop r18
 204 0050 0F90      		pop r0
 205 0052 0BBE      		out 91-32,r0
 206 0054 0F90      		pop r0
 207 0056 0FBE      		out __SREG__,r0
 208 0058 0F90      		pop r0
 209 005a 1F90      		pop __zero_reg__
 210 005c 1895      		reti
 211               		.size	__vector_4, .-__vector_4
 212               		.section	.text.__vector_5,"ax",@progbits
 213               	.global	__vector_5
 214               		.type	__vector_5, @function
 215               	__vector_5:
 216 0000 1F92      		push __zero_reg__
 217 0002 0F92      		push r0
 218 0004 0FB6      		in r0,__SREG__
 219 0006 0F92      		push r0
 220 0008 0BB6      		in r0,91-32
 221 000a 0F92      		push r0
 222 000c 1124      		clr __zero_reg__
 223 000e 2F93      		push r18
 224 0010 3F93      		push r19
 225 0012 4F93      		push r20
 226 0014 5F93      		push r21
 227 0016 6F93      		push r22
 228 0018 7F93      		push r23
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 5


 229 001a 8F93      		push r24
 230 001c 9F93      		push r25
 231 001e AF93      		push r26
 232 0020 BF93      		push r27
 233 0022 EF93      		push r30
 234 0024 FF93      		push r31
 235               	/* prologue: Signal */
 236               	/* frame size = 0 */
 237 0026 E091 0000 		lds r30,INT4_handler+2
 238 002a F091 0000 		lds r31,(INT4_handler+2)+1
 239 002e 8091 0000 		lds r24,INT4_handler
 240 0032 9091 0000 		lds r25,(INT4_handler)+1
 241 0036 0995      		icall
 242               	/* epilogue start */
 243 0038 FF91      		pop r31
 244 003a EF91      		pop r30
 245 003c BF91      		pop r27
 246 003e AF91      		pop r26
 247 0040 9F91      		pop r25
 248 0042 8F91      		pop r24
 249 0044 7F91      		pop r23
 250 0046 6F91      		pop r22
 251 0048 5F91      		pop r21
 252 004a 4F91      		pop r20
 253 004c 3F91      		pop r19
 254 004e 2F91      		pop r18
 255 0050 0F90      		pop r0
 256 0052 0BBE      		out 91-32,r0
 257 0054 0F90      		pop r0
 258 0056 0FBE      		out __SREG__,r0
 259 0058 0F90      		pop r0
 260 005a 1F90      		pop __zero_reg__
 261 005c 1895      		reti
 262               		.size	__vector_5, .-__vector_5
 263               		.section	.text.__vector_6,"ax",@progbits
 264               	.global	__vector_6
 265               		.type	__vector_6, @function
 266               	__vector_6:
 267 0000 1F92      		push __zero_reg__
 268 0002 0F92      		push r0
 269 0004 0FB6      		in r0,__SREG__
 270 0006 0F92      		push r0
 271 0008 0BB6      		in r0,91-32
 272 000a 0F92      		push r0
 273 000c 1124      		clr __zero_reg__
 274 000e 2F93      		push r18
 275 0010 3F93      		push r19
 276 0012 4F93      		push r20
 277 0014 5F93      		push r21
 278 0016 6F93      		push r22
 279 0018 7F93      		push r23
 280 001a 8F93      		push r24
 281 001c 9F93      		push r25
 282 001e AF93      		push r26
 283 0020 BF93      		push r27
 284 0022 EF93      		push r30
 285 0024 FF93      		push r31
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 6


 286               	/* prologue: Signal */
 287               	/* frame size = 0 */
 288 0026 E091 0000 		lds r30,INT5_handler+2
 289 002a F091 0000 		lds r31,(INT5_handler+2)+1
 290 002e 8091 0000 		lds r24,INT5_handler
 291 0032 9091 0000 		lds r25,(INT5_handler)+1
 292 0036 0995      		icall
 293               	/* epilogue start */
 294 0038 FF91      		pop r31
 295 003a EF91      		pop r30
 296 003c BF91      		pop r27
 297 003e AF91      		pop r26
 298 0040 9F91      		pop r25
 299 0042 8F91      		pop r24
 300 0044 7F91      		pop r23
 301 0046 6F91      		pop r22
 302 0048 5F91      		pop r21
 303 004a 4F91      		pop r20
 304 004c 3F91      		pop r19
 305 004e 2F91      		pop r18
 306 0050 0F90      		pop r0
 307 0052 0BBE      		out 91-32,r0
 308 0054 0F90      		pop r0
 309 0056 0FBE      		out __SREG__,r0
 310 0058 0F90      		pop r0
 311 005a 1F90      		pop __zero_reg__
 312 005c 1895      		reti
 313               		.size	__vector_6, .-__vector_6
 314               		.section	.text.__vector_7,"ax",@progbits
 315               	.global	__vector_7
 316               		.type	__vector_7, @function
 317               	__vector_7:
 318 0000 1F92      		push __zero_reg__
 319 0002 0F92      		push r0
 320 0004 0FB6      		in r0,__SREG__
 321 0006 0F92      		push r0
 322 0008 0BB6      		in r0,91-32
 323 000a 0F92      		push r0
 324 000c 1124      		clr __zero_reg__
 325 000e 2F93      		push r18
 326 0010 3F93      		push r19
 327 0012 4F93      		push r20
 328 0014 5F93      		push r21
 329 0016 6F93      		push r22
 330 0018 7F93      		push r23
 331 001a 8F93      		push r24
 332 001c 9F93      		push r25
 333 001e AF93      		push r26
 334 0020 BF93      		push r27
 335 0022 EF93      		push r30
 336 0024 FF93      		push r31
 337               	/* prologue: Signal */
 338               	/* frame size = 0 */
 339 0026 E091 0000 		lds r30,INT6_handler+2
 340 002a F091 0000 		lds r31,(INT6_handler+2)+1
 341 002e 8091 0000 		lds r24,INT6_handler
 342 0032 9091 0000 		lds r25,(INT6_handler)+1
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 7


 343 0036 0995      		icall
 344               	/* epilogue start */
 345 0038 FF91      		pop r31
 346 003a EF91      		pop r30
 347 003c BF91      		pop r27
 348 003e AF91      		pop r26
 349 0040 9F91      		pop r25
 350 0042 8F91      		pop r24
 351 0044 7F91      		pop r23
 352 0046 6F91      		pop r22
 353 0048 5F91      		pop r21
 354 004a 4F91      		pop r20
 355 004c 3F91      		pop r19
 356 004e 2F91      		pop r18
 357 0050 0F90      		pop r0
 358 0052 0BBE      		out 91-32,r0
 359 0054 0F90      		pop r0
 360 0056 0FBE      		out __SREG__,r0
 361 0058 0F90      		pop r0
 362 005a 1F90      		pop __zero_reg__
 363 005c 1895      		reti
 364               		.size	__vector_7, .-__vector_7
 365               		.section	.text.__vector_8,"ax",@progbits
 366               	.global	__vector_8
 367               		.type	__vector_8, @function
 368               	__vector_8:
 369 0000 1F92      		push __zero_reg__
 370 0002 0F92      		push r0
 371 0004 0FB6      		in r0,__SREG__
 372 0006 0F92      		push r0
 373 0008 0BB6      		in r0,91-32
 374 000a 0F92      		push r0
 375 000c 1124      		clr __zero_reg__
 376 000e 2F93      		push r18
 377 0010 3F93      		push r19
 378 0012 4F93      		push r20
 379 0014 5F93      		push r21
 380 0016 6F93      		push r22
 381 0018 7F93      		push r23
 382 001a 8F93      		push r24
 383 001c 9F93      		push r25
 384 001e AF93      		push r26
 385 0020 BF93      		push r27
 386 0022 EF93      		push r30
 387 0024 FF93      		push r31
 388               	/* prologue: Signal */
 389               	/* frame size = 0 */
 390 0026 E091 0000 		lds r30,INT7_handler+2
 391 002a F091 0000 		lds r31,(INT7_handler+2)+1
 392 002e 8091 0000 		lds r24,INT7_handler
 393 0032 9091 0000 		lds r25,(INT7_handler)+1
 394 0036 0995      		icall
 395               	/* epilogue start */
 396 0038 FF91      		pop r31
 397 003a EF91      		pop r30
 398 003c BF91      		pop r27
 399 003e AF91      		pop r26
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 8


 400 0040 9F91      		pop r25
 401 0042 8F91      		pop r24
 402 0044 7F91      		pop r23
 403 0046 6F91      		pop r22
 404 0048 5F91      		pop r21
 405 004a 4F91      		pop r20
 406 004c 3F91      		pop r19
 407 004e 2F91      		pop r18
 408 0050 0F90      		pop r0
 409 0052 0BBE      		out 91-32,r0
 410 0054 0F90      		pop r0
 411 0056 0FBE      		out __SREG__,r0
 412 0058 0F90      		pop r0
 413 005a 1F90      		pop __zero_reg__
 414 005c 1895      		reti
 415               		.size	__vector_8, .-__vector_8
 416               		.section	.text.__vector_9,"ax",@progbits
 417               	.global	__vector_9
 418               		.type	__vector_9, @function
 419               	__vector_9:
 420 0000 1F92      		push __zero_reg__
 421 0002 0F92      		push r0
 422 0004 0FB6      		in r0,__SREG__
 423 0006 0F92      		push r0
 424 0008 0BB6      		in r0,91-32
 425 000a 0F92      		push r0
 426 000c 1124      		clr __zero_reg__
 427 000e 2F93      		push r18
 428 0010 3F93      		push r19
 429 0012 4F93      		push r20
 430 0014 5F93      		push r21
 431 0016 6F93      		push r22
 432 0018 7F93      		push r23
 433 001a 8F93      		push r24
 434 001c 9F93      		push r25
 435 001e AF93      		push r26
 436 0020 BF93      		push r27
 437 0022 EF93      		push r30
 438 0024 FF93      		push r31
 439               	/* prologue: Signal */
 440               	/* frame size = 0 */
 441 0026 E091 0000 		lds r30,TIMER2_COMP_handler+2
 442 002a F091 0000 		lds r31,(TIMER2_COMP_handler+2)+1
 443 002e 8091 0000 		lds r24,TIMER2_COMP_handler
 444 0032 9091 0000 		lds r25,(TIMER2_COMP_handler)+1
 445 0036 0995      		icall
 446               	/* epilogue start */
 447 0038 FF91      		pop r31
 448 003a EF91      		pop r30
 449 003c BF91      		pop r27
 450 003e AF91      		pop r26
 451 0040 9F91      		pop r25
 452 0042 8F91      		pop r24
 453 0044 7F91      		pop r23
 454 0046 6F91      		pop r22
 455 0048 5F91      		pop r21
 456 004a 4F91      		pop r20
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 9


 457 004c 3F91      		pop r19
 458 004e 2F91      		pop r18
 459 0050 0F90      		pop r0
 460 0052 0BBE      		out 91-32,r0
 461 0054 0F90      		pop r0
 462 0056 0FBE      		out __SREG__,r0
 463 0058 0F90      		pop r0
 464 005a 1F90      		pop __zero_reg__
 465 005c 1895      		reti
 466               		.size	__vector_9, .-__vector_9
 467               		.section	.text.__vector_10,"ax",@progbits
 468               	.global	__vector_10
 469               		.type	__vector_10, @function
 470               	__vector_10:
 471 0000 1F92      		push __zero_reg__
 472 0002 0F92      		push r0
 473 0004 0FB6      		in r0,__SREG__
 474 0006 0F92      		push r0
 475 0008 0BB6      		in r0,91-32
 476 000a 0F92      		push r0
 477 000c 1124      		clr __zero_reg__
 478 000e 2F93      		push r18
 479 0010 3F93      		push r19
 480 0012 4F93      		push r20
 481 0014 5F93      		push r21
 482 0016 6F93      		push r22
 483 0018 7F93      		push r23
 484 001a 8F93      		push r24
 485 001c 9F93      		push r25
 486 001e AF93      		push r26
 487 0020 BF93      		push r27
 488 0022 EF93      		push r30
 489 0024 FF93      		push r31
 490               	/* prologue: Signal */
 491               	/* frame size = 0 */
 492 0026 E091 0000 		lds r30,TIMER2_OVF_handler+2
 493 002a F091 0000 		lds r31,(TIMER2_OVF_handler+2)+1
 494 002e 8091 0000 		lds r24,TIMER2_OVF_handler
 495 0032 9091 0000 		lds r25,(TIMER2_OVF_handler)+1
 496 0036 0995      		icall
 497               	/* epilogue start */
 498 0038 FF91      		pop r31
 499 003a EF91      		pop r30
 500 003c BF91      		pop r27
 501 003e AF91      		pop r26
 502 0040 9F91      		pop r25
 503 0042 8F91      		pop r24
 504 0044 7F91      		pop r23
 505 0046 6F91      		pop r22
 506 0048 5F91      		pop r21
 507 004a 4F91      		pop r20
 508 004c 3F91      		pop r19
 509 004e 2F91      		pop r18
 510 0050 0F90      		pop r0
 511 0052 0BBE      		out 91-32,r0
 512 0054 0F90      		pop r0
 513 0056 0FBE      		out __SREG__,r0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 10


 514 0058 0F90      		pop r0
 515 005a 1F90      		pop __zero_reg__
 516 005c 1895      		reti
 517               		.size	__vector_10, .-__vector_10
 518               		.section	.text.__vector_11,"ax",@progbits
 519               	.global	__vector_11
 520               		.type	__vector_11, @function
 521               	__vector_11:
 522 0000 1F92      		push __zero_reg__
 523 0002 0F92      		push r0
 524 0004 0FB6      		in r0,__SREG__
 525 0006 0F92      		push r0
 526 0008 0BB6      		in r0,91-32
 527 000a 0F92      		push r0
 528 000c 1124      		clr __zero_reg__
 529 000e 2F93      		push r18
 530 0010 3F93      		push r19
 531 0012 4F93      		push r20
 532 0014 5F93      		push r21
 533 0016 6F93      		push r22
 534 0018 7F93      		push r23
 535 001a 8F93      		push r24
 536 001c 9F93      		push r25
 537 001e AF93      		push r26
 538 0020 BF93      		push r27
 539 0022 EF93      		push r30
 540 0024 FF93      		push r31
 541               	/* prologue: Signal */
 542               	/* frame size = 0 */
 543 0026 E091 0000 		lds r30,TIMER1_CAPT_handler+2
 544 002a F091 0000 		lds r31,(TIMER1_CAPT_handler+2)+1
 545 002e 8091 0000 		lds r24,TIMER1_CAPT_handler
 546 0032 9091 0000 		lds r25,(TIMER1_CAPT_handler)+1
 547 0036 0995      		icall
 548               	/* epilogue start */
 549 0038 FF91      		pop r31
 550 003a EF91      		pop r30
 551 003c BF91      		pop r27
 552 003e AF91      		pop r26
 553 0040 9F91      		pop r25
 554 0042 8F91      		pop r24
 555 0044 7F91      		pop r23
 556 0046 6F91      		pop r22
 557 0048 5F91      		pop r21
 558 004a 4F91      		pop r20
 559 004c 3F91      		pop r19
 560 004e 2F91      		pop r18
 561 0050 0F90      		pop r0
 562 0052 0BBE      		out 91-32,r0
 563 0054 0F90      		pop r0
 564 0056 0FBE      		out __SREG__,r0
 565 0058 0F90      		pop r0
 566 005a 1F90      		pop __zero_reg__
 567 005c 1895      		reti
 568               		.size	__vector_11, .-__vector_11
 569               		.section	.text.__vector_12,"ax",@progbits
 570               	.global	__vector_12
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 11


 571               		.type	__vector_12, @function
 572               	__vector_12:
 573 0000 1F92      		push __zero_reg__
 574 0002 0F92      		push r0
 575 0004 0FB6      		in r0,__SREG__
 576 0006 0F92      		push r0
 577 0008 0BB6      		in r0,91-32
 578 000a 0F92      		push r0
 579 000c 1124      		clr __zero_reg__
 580 000e 2F93      		push r18
 581 0010 3F93      		push r19
 582 0012 4F93      		push r20
 583 0014 5F93      		push r21
 584 0016 6F93      		push r22
 585 0018 7F93      		push r23
 586 001a 8F93      		push r24
 587 001c 9F93      		push r25
 588 001e AF93      		push r26
 589 0020 BF93      		push r27
 590 0022 EF93      		push r30
 591 0024 FF93      		push r31
 592               	/* prologue: Signal */
 593               	/* frame size = 0 */
 594 0026 E091 0000 		lds r30,TIMER1_COMPA_handler+2
 595 002a F091 0000 		lds r31,(TIMER1_COMPA_handler+2)+1
 596 002e 8091 0000 		lds r24,TIMER1_COMPA_handler
 597 0032 9091 0000 		lds r25,(TIMER1_COMPA_handler)+1
 598 0036 0995      		icall
 599               	/* epilogue start */
 600 0038 FF91      		pop r31
 601 003a EF91      		pop r30
 602 003c BF91      		pop r27
 603 003e AF91      		pop r26
 604 0040 9F91      		pop r25
 605 0042 8F91      		pop r24
 606 0044 7F91      		pop r23
 607 0046 6F91      		pop r22
 608 0048 5F91      		pop r21
 609 004a 4F91      		pop r20
 610 004c 3F91      		pop r19
 611 004e 2F91      		pop r18
 612 0050 0F90      		pop r0
 613 0052 0BBE      		out 91-32,r0
 614 0054 0F90      		pop r0
 615 0056 0FBE      		out __SREG__,r0
 616 0058 0F90      		pop r0
 617 005a 1F90      		pop __zero_reg__
 618 005c 1895      		reti
 619               		.size	__vector_12, .-__vector_12
 620               		.section	.text.__vector_13,"ax",@progbits
 621               	.global	__vector_13
 622               		.type	__vector_13, @function
 623               	__vector_13:
 624 0000 1F92      		push __zero_reg__
 625 0002 0F92      		push r0
 626 0004 0FB6      		in r0,__SREG__
 627 0006 0F92      		push r0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 12


 628 0008 0BB6      		in r0,91-32
 629 000a 0F92      		push r0
 630 000c 1124      		clr __zero_reg__
 631 000e 2F93      		push r18
 632 0010 3F93      		push r19
 633 0012 4F93      		push r20
 634 0014 5F93      		push r21
 635 0016 6F93      		push r22
 636 0018 7F93      		push r23
 637 001a 8F93      		push r24
 638 001c 9F93      		push r25
 639 001e AF93      		push r26
 640 0020 BF93      		push r27
 641 0022 EF93      		push r30
 642 0024 FF93      		push r31
 643               	/* prologue: Signal */
 644               	/* frame size = 0 */
 645 0026 E091 0000 		lds r30,TIMER1_COMPB_handler+2
 646 002a F091 0000 		lds r31,(TIMER1_COMPB_handler+2)+1
 647 002e 8091 0000 		lds r24,TIMER1_COMPB_handler
 648 0032 9091 0000 		lds r25,(TIMER1_COMPB_handler)+1
 649 0036 0995      		icall
 650               	/* epilogue start */
 651 0038 FF91      		pop r31
 652 003a EF91      		pop r30
 653 003c BF91      		pop r27
 654 003e AF91      		pop r26
 655 0040 9F91      		pop r25
 656 0042 8F91      		pop r24
 657 0044 7F91      		pop r23
 658 0046 6F91      		pop r22
 659 0048 5F91      		pop r21
 660 004a 4F91      		pop r20
 661 004c 3F91      		pop r19
 662 004e 2F91      		pop r18
 663 0050 0F90      		pop r0
 664 0052 0BBE      		out 91-32,r0
 665 0054 0F90      		pop r0
 666 0056 0FBE      		out __SREG__,r0
 667 0058 0F90      		pop r0
 668 005a 1F90      		pop __zero_reg__
 669 005c 1895      		reti
 670               		.size	__vector_13, .-__vector_13
 671               		.section	.text.__vector_14,"ax",@progbits
 672               	.global	__vector_14
 673               		.type	__vector_14, @function
 674               	__vector_14:
 675 0000 1F92      		push __zero_reg__
 676 0002 0F92      		push r0
 677 0004 0FB6      		in r0,__SREG__
 678 0006 0F92      		push r0
 679 0008 0BB6      		in r0,91-32
 680 000a 0F92      		push r0
 681 000c 1124      		clr __zero_reg__
 682 000e 2F93      		push r18
 683 0010 3F93      		push r19
 684 0012 4F93      		push r20
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 13


 685 0014 5F93      		push r21
 686 0016 6F93      		push r22
 687 0018 7F93      		push r23
 688 001a 8F93      		push r24
 689 001c 9F93      		push r25
 690 001e AF93      		push r26
 691 0020 BF93      		push r27
 692 0022 EF93      		push r30
 693 0024 FF93      		push r31
 694               	/* prologue: Signal */
 695               	/* frame size = 0 */
 696 0026 E091 0000 		lds r30,TIMER1_COMPC_handler+2
 697 002a F091 0000 		lds r31,(TIMER1_COMPC_handler+2)+1
 698 002e 8091 0000 		lds r24,TIMER1_COMPC_handler
 699 0032 9091 0000 		lds r25,(TIMER1_COMPC_handler)+1
 700 0036 0995      		icall
 701               	/* epilogue start */
 702 0038 FF91      		pop r31
 703 003a EF91      		pop r30
 704 003c BF91      		pop r27
 705 003e AF91      		pop r26
 706 0040 9F91      		pop r25
 707 0042 8F91      		pop r24
 708 0044 7F91      		pop r23
 709 0046 6F91      		pop r22
 710 0048 5F91      		pop r21
 711 004a 4F91      		pop r20
 712 004c 3F91      		pop r19
 713 004e 2F91      		pop r18
 714 0050 0F90      		pop r0
 715 0052 0BBE      		out 91-32,r0
 716 0054 0F90      		pop r0
 717 0056 0FBE      		out __SREG__,r0
 718 0058 0F90      		pop r0
 719 005a 1F90      		pop __zero_reg__
 720 005c 1895      		reti
 721               		.size	__vector_14, .-__vector_14
 722               		.section	.text.__vector_15,"ax",@progbits
 723               	.global	__vector_15
 724               		.type	__vector_15, @function
 725               	__vector_15:
 726 0000 1F92      		push __zero_reg__
 727 0002 0F92      		push r0
 728 0004 0FB6      		in r0,__SREG__
 729 0006 0F92      		push r0
 730 0008 0BB6      		in r0,91-32
 731 000a 0F92      		push r0
 732 000c 1124      		clr __zero_reg__
 733 000e 2F93      		push r18
 734 0010 3F93      		push r19
 735 0012 4F93      		push r20
 736 0014 5F93      		push r21
 737 0016 6F93      		push r22
 738 0018 7F93      		push r23
 739 001a 8F93      		push r24
 740 001c 9F93      		push r25
 741 001e AF93      		push r26
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 14


 742 0020 BF93      		push r27
 743 0022 EF93      		push r30
 744 0024 FF93      		push r31
 745               	/* prologue: Signal */
 746               	/* frame size = 0 */
 747 0026 E091 0000 		lds r30,TIMER1_OVF_handler+2
 748 002a F091 0000 		lds r31,(TIMER1_OVF_handler+2)+1
 749 002e 8091 0000 		lds r24,TIMER1_OVF_handler
 750 0032 9091 0000 		lds r25,(TIMER1_OVF_handler)+1
 751 0036 0995      		icall
 752               	/* epilogue start */
 753 0038 FF91      		pop r31
 754 003a EF91      		pop r30
 755 003c BF91      		pop r27
 756 003e AF91      		pop r26
 757 0040 9F91      		pop r25
 758 0042 8F91      		pop r24
 759 0044 7F91      		pop r23
 760 0046 6F91      		pop r22
 761 0048 5F91      		pop r21
 762 004a 4F91      		pop r20
 763 004c 3F91      		pop r19
 764 004e 2F91      		pop r18
 765 0050 0F90      		pop r0
 766 0052 0BBE      		out 91-32,r0
 767 0054 0F90      		pop r0
 768 0056 0FBE      		out __SREG__,r0
 769 0058 0F90      		pop r0
 770 005a 1F90      		pop __zero_reg__
 771 005c 1895      		reti
 772               		.size	__vector_15, .-__vector_15
 773               		.section	.text.__vector_16,"ax",@progbits
 774               	.global	__vector_16
 775               		.type	__vector_16, @function
 776               	__vector_16:
 777 0000 1F92      		push __zero_reg__
 778 0002 0F92      		push r0
 779 0004 0FB6      		in r0,__SREG__
 780 0006 0F92      		push r0
 781 0008 0BB6      		in r0,91-32
 782 000a 0F92      		push r0
 783 000c 1124      		clr __zero_reg__
 784 000e 2F93      		push r18
 785 0010 3F93      		push r19
 786 0012 4F93      		push r20
 787 0014 5F93      		push r21
 788 0016 6F93      		push r22
 789 0018 7F93      		push r23
 790 001a 8F93      		push r24
 791 001c 9F93      		push r25
 792 001e AF93      		push r26
 793 0020 BF93      		push r27
 794 0022 EF93      		push r30
 795 0024 FF93      		push r31
 796               	/* prologue: Signal */
 797               	/* frame size = 0 */
 798 0026 E091 0000 		lds r30,TIMER0_COMP_handler+2
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 15


 799 002a F091 0000 		lds r31,(TIMER0_COMP_handler+2)+1
 800 002e 8091 0000 		lds r24,TIMER0_COMP_handler
 801 0032 9091 0000 		lds r25,(TIMER0_COMP_handler)+1
 802 0036 0995      		icall
 803               	/* epilogue start */
 804 0038 FF91      		pop r31
 805 003a EF91      		pop r30
 806 003c BF91      		pop r27
 807 003e AF91      		pop r26
 808 0040 9F91      		pop r25
 809 0042 8F91      		pop r24
 810 0044 7F91      		pop r23
 811 0046 6F91      		pop r22
 812 0048 5F91      		pop r21
 813 004a 4F91      		pop r20
 814 004c 3F91      		pop r19
 815 004e 2F91      		pop r18
 816 0050 0F90      		pop r0
 817 0052 0BBE      		out 91-32,r0
 818 0054 0F90      		pop r0
 819 0056 0FBE      		out __SREG__,r0
 820 0058 0F90      		pop r0
 821 005a 1F90      		pop __zero_reg__
 822 005c 1895      		reti
 823               		.size	__vector_16, .-__vector_16
 824               		.section	.text.__vector_17,"ax",@progbits
 825               	.global	__vector_17
 826               		.type	__vector_17, @function
 827               	__vector_17:
 828 0000 1F92      		push __zero_reg__
 829 0002 0F92      		push r0
 830 0004 0FB6      		in r0,__SREG__
 831 0006 0F92      		push r0
 832 0008 0BB6      		in r0,91-32
 833 000a 0F92      		push r0
 834 000c 1124      		clr __zero_reg__
 835 000e 2F93      		push r18
 836 0010 3F93      		push r19
 837 0012 4F93      		push r20
 838 0014 5F93      		push r21
 839 0016 6F93      		push r22
 840 0018 7F93      		push r23
 841 001a 8F93      		push r24
 842 001c 9F93      		push r25
 843 001e AF93      		push r26
 844 0020 BF93      		push r27
 845 0022 EF93      		push r30
 846 0024 FF93      		push r31
 847               	/* prologue: Signal */
 848               	/* frame size = 0 */
 849 0026 E091 0000 		lds r30,TIMER0_OVF_handler+2
 850 002a F091 0000 		lds r31,(TIMER0_OVF_handler+2)+1
 851 002e 8091 0000 		lds r24,TIMER0_OVF_handler
 852 0032 9091 0000 		lds r25,(TIMER0_OVF_handler)+1
 853 0036 0995      		icall
 854               	/* epilogue start */
 855 0038 FF91      		pop r31
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 16


 856 003a EF91      		pop r30
 857 003c BF91      		pop r27
 858 003e AF91      		pop r26
 859 0040 9F91      		pop r25
 860 0042 8F91      		pop r24
 861 0044 7F91      		pop r23
 862 0046 6F91      		pop r22
 863 0048 5F91      		pop r21
 864 004a 4F91      		pop r20
 865 004c 3F91      		pop r19
 866 004e 2F91      		pop r18
 867 0050 0F90      		pop r0
 868 0052 0BBE      		out 91-32,r0
 869 0054 0F90      		pop r0
 870 0056 0FBE      		out __SREG__,r0
 871 0058 0F90      		pop r0
 872 005a 1F90      		pop __zero_reg__
 873 005c 1895      		reti
 874               		.size	__vector_17, .-__vector_17
 875               		.section	.text.__vector_18,"ax",@progbits
 876               	.global	__vector_18
 877               		.type	__vector_18, @function
 878               	__vector_18:
 879 0000 1F92      		push __zero_reg__
 880 0002 0F92      		push r0
 881 0004 0FB6      		in r0,__SREG__
 882 0006 0F92      		push r0
 883 0008 0BB6      		in r0,91-32
 884 000a 0F92      		push r0
 885 000c 1124      		clr __zero_reg__
 886 000e 2F93      		push r18
 887 0010 3F93      		push r19
 888 0012 4F93      		push r20
 889 0014 5F93      		push r21
 890 0016 6F93      		push r22
 891 0018 7F93      		push r23
 892 001a 8F93      		push r24
 893 001c 9F93      		push r25
 894 001e AF93      		push r26
 895 0020 BF93      		push r27
 896 0022 EF93      		push r30
 897 0024 FF93      		push r31
 898               	/* prologue: Signal */
 899               	/* frame size = 0 */
 900 0026 E091 0000 		lds r30,CANIT_handler+2
 901 002a F091 0000 		lds r31,(CANIT_handler+2)+1
 902 002e 8091 0000 		lds r24,CANIT_handler
 903 0032 9091 0000 		lds r25,(CANIT_handler)+1
 904 0036 0995      		icall
 905               	/* epilogue start */
 906 0038 FF91      		pop r31
 907 003a EF91      		pop r30
 908 003c BF91      		pop r27
 909 003e AF91      		pop r26
 910 0040 9F91      		pop r25
 911 0042 8F91      		pop r24
 912 0044 7F91      		pop r23
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 17


 913 0046 6F91      		pop r22
 914 0048 5F91      		pop r21
 915 004a 4F91      		pop r20
 916 004c 3F91      		pop r19
 917 004e 2F91      		pop r18
 918 0050 0F90      		pop r0
 919 0052 0BBE      		out 91-32,r0
 920 0054 0F90      		pop r0
 921 0056 0FBE      		out __SREG__,r0
 922 0058 0F90      		pop r0
 923 005a 1F90      		pop __zero_reg__
 924 005c 1895      		reti
 925               		.size	__vector_18, .-__vector_18
 926               		.section	.text.__vector_19,"ax",@progbits
 927               	.global	__vector_19
 928               		.type	__vector_19, @function
 929               	__vector_19:
 930 0000 1F92      		push __zero_reg__
 931 0002 0F92      		push r0
 932 0004 0FB6      		in r0,__SREG__
 933 0006 0F92      		push r0
 934 0008 0BB6      		in r0,91-32
 935 000a 0F92      		push r0
 936 000c 1124      		clr __zero_reg__
 937 000e 2F93      		push r18
 938 0010 3F93      		push r19
 939 0012 4F93      		push r20
 940 0014 5F93      		push r21
 941 0016 6F93      		push r22
 942 0018 7F93      		push r23
 943 001a 8F93      		push r24
 944 001c 9F93      		push r25
 945 001e AF93      		push r26
 946 0020 BF93      		push r27
 947 0022 EF93      		push r30
 948 0024 FF93      		push r31
 949               	/* prologue: Signal */
 950               	/* frame size = 0 */
 951 0026 E091 0000 		lds r30,OVRIT_handler+2
 952 002a F091 0000 		lds r31,(OVRIT_handler+2)+1
 953 002e 8091 0000 		lds r24,OVRIT_handler
 954 0032 9091 0000 		lds r25,(OVRIT_handler)+1
 955 0036 0995      		icall
 956               	/* epilogue start */
 957 0038 FF91      		pop r31
 958 003a EF91      		pop r30
 959 003c BF91      		pop r27
 960 003e AF91      		pop r26
 961 0040 9F91      		pop r25
 962 0042 8F91      		pop r24
 963 0044 7F91      		pop r23
 964 0046 6F91      		pop r22
 965 0048 5F91      		pop r21
 966 004a 4F91      		pop r20
 967 004c 3F91      		pop r19
 968 004e 2F91      		pop r18
 969 0050 0F90      		pop r0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 18


 970 0052 0BBE      		out 91-32,r0
 971 0054 0F90      		pop r0
 972 0056 0FBE      		out __SREG__,r0
 973 0058 0F90      		pop r0
 974 005a 1F90      		pop __zero_reg__
 975 005c 1895      		reti
 976               		.size	__vector_19, .-__vector_19
 977               		.section	.text.__vector_20,"ax",@progbits
 978               	.global	__vector_20
 979               		.type	__vector_20, @function
 980               	__vector_20:
 981 0000 1F92      		push __zero_reg__
 982 0002 0F92      		push r0
 983 0004 0FB6      		in r0,__SREG__
 984 0006 0F92      		push r0
 985 0008 0BB6      		in r0,91-32
 986 000a 0F92      		push r0
 987 000c 1124      		clr __zero_reg__
 988 000e 2F93      		push r18
 989 0010 3F93      		push r19
 990 0012 4F93      		push r20
 991 0014 5F93      		push r21
 992 0016 6F93      		push r22
 993 0018 7F93      		push r23
 994 001a 8F93      		push r24
 995 001c 9F93      		push r25
 996 001e AF93      		push r26
 997 0020 BF93      		push r27
 998 0022 EF93      		push r30
 999 0024 FF93      		push r31
 1000               	/* prologue: Signal */
 1001               	/* frame size = 0 */
 1002 0026 E091 0000 		lds r30,SPI_STC_handler+2
 1003 002a F091 0000 		lds r31,(SPI_STC_handler+2)+1
 1004 002e 8091 0000 		lds r24,SPI_STC_handler
 1005 0032 9091 0000 		lds r25,(SPI_STC_handler)+1
 1006 0036 0995      		icall
 1007               	/* epilogue start */
 1008 0038 FF91      		pop r31
 1009 003a EF91      		pop r30
 1010 003c BF91      		pop r27
 1011 003e AF91      		pop r26
 1012 0040 9F91      		pop r25
 1013 0042 8F91      		pop r24
 1014 0044 7F91      		pop r23
 1015 0046 6F91      		pop r22
 1016 0048 5F91      		pop r21
 1017 004a 4F91      		pop r20
 1018 004c 3F91      		pop r19
 1019 004e 2F91      		pop r18
 1020 0050 0F90      		pop r0
 1021 0052 0BBE      		out 91-32,r0
 1022 0054 0F90      		pop r0
 1023 0056 0FBE      		out __SREG__,r0
 1024 0058 0F90      		pop r0
 1025 005a 1F90      		pop __zero_reg__
 1026 005c 1895      		reti
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 19


 1027               		.size	__vector_20, .-__vector_20
 1028               		.section	.text.__vector_21,"ax",@progbits
 1029               	.global	__vector_21
 1030               		.type	__vector_21, @function
 1031               	__vector_21:
 1032 0000 1F92      		push __zero_reg__
 1033 0002 0F92      		push r0
 1034 0004 0FB6      		in r0,__SREG__
 1035 0006 0F92      		push r0
 1036 0008 0BB6      		in r0,91-32
 1037 000a 0F92      		push r0
 1038 000c 1124      		clr __zero_reg__
 1039 000e 2F93      		push r18
 1040 0010 3F93      		push r19
 1041 0012 4F93      		push r20
 1042 0014 5F93      		push r21
 1043 0016 6F93      		push r22
 1044 0018 7F93      		push r23
 1045 001a 8F93      		push r24
 1046 001c 9F93      		push r25
 1047 001e AF93      		push r26
 1048 0020 BF93      		push r27
 1049 0022 EF93      		push r30
 1050 0024 FF93      		push r31
 1051               	/* prologue: Signal */
 1052               	/* frame size = 0 */
 1053 0026 E091 0000 		lds r30,USART0_RX_handler+2
 1054 002a F091 0000 		lds r31,(USART0_RX_handler+2)+1
 1055 002e 8091 0000 		lds r24,USART0_RX_handler
 1056 0032 9091 0000 		lds r25,(USART0_RX_handler)+1
 1057 0036 0995      		icall
 1058               	/* epilogue start */
 1059 0038 FF91      		pop r31
 1060 003a EF91      		pop r30
 1061 003c BF91      		pop r27
 1062 003e AF91      		pop r26
 1063 0040 9F91      		pop r25
 1064 0042 8F91      		pop r24
 1065 0044 7F91      		pop r23
 1066 0046 6F91      		pop r22
 1067 0048 5F91      		pop r21
 1068 004a 4F91      		pop r20
 1069 004c 3F91      		pop r19
 1070 004e 2F91      		pop r18
 1071 0050 0F90      		pop r0
 1072 0052 0BBE      		out 91-32,r0
 1073 0054 0F90      		pop r0
 1074 0056 0FBE      		out __SREG__,r0
 1075 0058 0F90      		pop r0
 1076 005a 1F90      		pop __zero_reg__
 1077 005c 1895      		reti
 1078               		.size	__vector_21, .-__vector_21
 1079               		.section	.text.__vector_22,"ax",@progbits
 1080               	.global	__vector_22
 1081               		.type	__vector_22, @function
 1082               	__vector_22:
 1083 0000 1F92      		push __zero_reg__
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 20


 1084 0002 0F92      		push r0
 1085 0004 0FB6      		in r0,__SREG__
 1086 0006 0F92      		push r0
 1087 0008 0BB6      		in r0,91-32
 1088 000a 0F92      		push r0
 1089 000c 1124      		clr __zero_reg__
 1090 000e 2F93      		push r18
 1091 0010 3F93      		push r19
 1092 0012 4F93      		push r20
 1093 0014 5F93      		push r21
 1094 0016 6F93      		push r22
 1095 0018 7F93      		push r23
 1096 001a 8F93      		push r24
 1097 001c 9F93      		push r25
 1098 001e AF93      		push r26
 1099 0020 BF93      		push r27
 1100 0022 EF93      		push r30
 1101 0024 FF93      		push r31
 1102               	/* prologue: Signal */
 1103               	/* frame size = 0 */
 1104 0026 E091 0000 		lds r30,USART0_UDRE_handler+2
 1105 002a F091 0000 		lds r31,(USART0_UDRE_handler+2)+1
 1106 002e 8091 0000 		lds r24,USART0_UDRE_handler
 1107 0032 9091 0000 		lds r25,(USART0_UDRE_handler)+1
 1108 0036 0995      		icall
 1109               	/* epilogue start */
 1110 0038 FF91      		pop r31
 1111 003a EF91      		pop r30
 1112 003c BF91      		pop r27
 1113 003e AF91      		pop r26
 1114 0040 9F91      		pop r25
 1115 0042 8F91      		pop r24
 1116 0044 7F91      		pop r23
 1117 0046 6F91      		pop r22
 1118 0048 5F91      		pop r21
 1119 004a 4F91      		pop r20
 1120 004c 3F91      		pop r19
 1121 004e 2F91      		pop r18
 1122 0050 0F90      		pop r0
 1123 0052 0BBE      		out 91-32,r0
 1124 0054 0F90      		pop r0
 1125 0056 0FBE      		out __SREG__,r0
 1126 0058 0F90      		pop r0
 1127 005a 1F90      		pop __zero_reg__
 1128 005c 1895      		reti
 1129               		.size	__vector_22, .-__vector_22
 1130               		.section	.text.__vector_23,"ax",@progbits
 1131               	.global	__vector_23
 1132               		.type	__vector_23, @function
 1133               	__vector_23:
 1134 0000 1F92      		push __zero_reg__
 1135 0002 0F92      		push r0
 1136 0004 0FB6      		in r0,__SREG__
 1137 0006 0F92      		push r0
 1138 0008 0BB6      		in r0,91-32
 1139 000a 0F92      		push r0
 1140 000c 1124      		clr __zero_reg__
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 21


 1141 000e 2F93      		push r18
 1142 0010 3F93      		push r19
 1143 0012 4F93      		push r20
 1144 0014 5F93      		push r21
 1145 0016 6F93      		push r22
 1146 0018 7F93      		push r23
 1147 001a 8F93      		push r24
 1148 001c 9F93      		push r25
 1149 001e AF93      		push r26
 1150 0020 BF93      		push r27
 1151 0022 EF93      		push r30
 1152 0024 FF93      		push r31
 1153               	/* prologue: Signal */
 1154               	/* frame size = 0 */
 1155 0026 E091 0000 		lds r30,USART0_TX_handler+2
 1156 002a F091 0000 		lds r31,(USART0_TX_handler+2)+1
 1157 002e 8091 0000 		lds r24,USART0_TX_handler
 1158 0032 9091 0000 		lds r25,(USART0_TX_handler)+1
 1159 0036 0995      		icall
 1160               	/* epilogue start */
 1161 0038 FF91      		pop r31
 1162 003a EF91      		pop r30
 1163 003c BF91      		pop r27
 1164 003e AF91      		pop r26
 1165 0040 9F91      		pop r25
 1166 0042 8F91      		pop r24
 1167 0044 7F91      		pop r23
 1168 0046 6F91      		pop r22
 1169 0048 5F91      		pop r21
 1170 004a 4F91      		pop r20
 1171 004c 3F91      		pop r19
 1172 004e 2F91      		pop r18
 1173 0050 0F90      		pop r0
 1174 0052 0BBE      		out 91-32,r0
 1175 0054 0F90      		pop r0
 1176 0056 0FBE      		out __SREG__,r0
 1177 0058 0F90      		pop r0
 1178 005a 1F90      		pop __zero_reg__
 1179 005c 1895      		reti
 1180               		.size	__vector_23, .-__vector_23
 1181               		.section	.text.__vector_24,"ax",@progbits
 1182               	.global	__vector_24
 1183               		.type	__vector_24, @function
 1184               	__vector_24:
 1185 0000 1F92      		push __zero_reg__
 1186 0002 0F92      		push r0
 1187 0004 0FB6      		in r0,__SREG__
 1188 0006 0F92      		push r0
 1189 0008 0BB6      		in r0,91-32
 1190 000a 0F92      		push r0
 1191 000c 1124      		clr __zero_reg__
 1192 000e 2F93      		push r18
 1193 0010 3F93      		push r19
 1194 0012 4F93      		push r20
 1195 0014 5F93      		push r21
 1196 0016 6F93      		push r22
 1197 0018 7F93      		push r23
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 22


 1198 001a 8F93      		push r24
 1199 001c 9F93      		push r25
 1200 001e AF93      		push r26
 1201 0020 BF93      		push r27
 1202 0022 EF93      		push r30
 1203 0024 FF93      		push r31
 1204               	/* prologue: Signal */
 1205               	/* frame size = 0 */
 1206 0026 E091 0000 		lds r30,ANALOG_COMP_handler+2
 1207 002a F091 0000 		lds r31,(ANALOG_COMP_handler+2)+1
 1208 002e 8091 0000 		lds r24,ANALOG_COMP_handler
 1209 0032 9091 0000 		lds r25,(ANALOG_COMP_handler)+1
 1210 0036 0995      		icall
 1211               	/* epilogue start */
 1212 0038 FF91      		pop r31
 1213 003a EF91      		pop r30
 1214 003c BF91      		pop r27
 1215 003e AF91      		pop r26
 1216 0040 9F91      		pop r25
 1217 0042 8F91      		pop r24
 1218 0044 7F91      		pop r23
 1219 0046 6F91      		pop r22
 1220 0048 5F91      		pop r21
 1221 004a 4F91      		pop r20
 1222 004c 3F91      		pop r19
 1223 004e 2F91      		pop r18
 1224 0050 0F90      		pop r0
 1225 0052 0BBE      		out 91-32,r0
 1226 0054 0F90      		pop r0
 1227 0056 0FBE      		out __SREG__,r0
 1228 0058 0F90      		pop r0
 1229 005a 1F90      		pop __zero_reg__
 1230 005c 1895      		reti
 1231               		.size	__vector_24, .-__vector_24
 1232               		.section	.text.__vector_25,"ax",@progbits
 1233               	.global	__vector_25
 1234               		.type	__vector_25, @function
 1235               	__vector_25:
 1236 0000 1F92      		push __zero_reg__
 1237 0002 0F92      		push r0
 1238 0004 0FB6      		in r0,__SREG__
 1239 0006 0F92      		push r0
 1240 0008 0BB6      		in r0,91-32
 1241 000a 0F92      		push r0
 1242 000c 1124      		clr __zero_reg__
 1243 000e 2F93      		push r18
 1244 0010 3F93      		push r19
 1245 0012 4F93      		push r20
 1246 0014 5F93      		push r21
 1247 0016 6F93      		push r22
 1248 0018 7F93      		push r23
 1249 001a 8F93      		push r24
 1250 001c 9F93      		push r25
 1251 001e AF93      		push r26
 1252 0020 BF93      		push r27
 1253 0022 EF93      		push r30
 1254 0024 FF93      		push r31
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 23


 1255               	/* prologue: Signal */
 1256               	/* frame size = 0 */
 1257 0026 E091 0000 		lds r30,ADC_handler+2
 1258 002a F091 0000 		lds r31,(ADC_handler+2)+1
 1259 002e 8091 0000 		lds r24,ADC_handler
 1260 0032 9091 0000 		lds r25,(ADC_handler)+1
 1261 0036 0995      		icall
 1262               	/* epilogue start */
 1263 0038 FF91      		pop r31
 1264 003a EF91      		pop r30
 1265 003c BF91      		pop r27
 1266 003e AF91      		pop r26
 1267 0040 9F91      		pop r25
 1268 0042 8F91      		pop r24
 1269 0044 7F91      		pop r23
 1270 0046 6F91      		pop r22
 1271 0048 5F91      		pop r21
 1272 004a 4F91      		pop r20
 1273 004c 3F91      		pop r19
 1274 004e 2F91      		pop r18
 1275 0050 0F90      		pop r0
 1276 0052 0BBE      		out 91-32,r0
 1277 0054 0F90      		pop r0
 1278 0056 0FBE      		out __SREG__,r0
 1279 0058 0F90      		pop r0
 1280 005a 1F90      		pop __zero_reg__
 1281 005c 1895      		reti
 1282               		.size	__vector_25, .-__vector_25
 1283               		.section	.text.__vector_26,"ax",@progbits
 1284               	.global	__vector_26
 1285               		.type	__vector_26, @function
 1286               	__vector_26:
 1287 0000 1F92      		push __zero_reg__
 1288 0002 0F92      		push r0
 1289 0004 0FB6      		in r0,__SREG__
 1290 0006 0F92      		push r0
 1291 0008 0BB6      		in r0,91-32
 1292 000a 0F92      		push r0
 1293 000c 1124      		clr __zero_reg__
 1294 000e 2F93      		push r18
 1295 0010 3F93      		push r19
 1296 0012 4F93      		push r20
 1297 0014 5F93      		push r21
 1298 0016 6F93      		push r22
 1299 0018 7F93      		push r23
 1300 001a 8F93      		push r24
 1301 001c 9F93      		push r25
 1302 001e AF93      		push r26
 1303 0020 BF93      		push r27
 1304 0022 EF93      		push r30
 1305 0024 FF93      		push r31
 1306               	/* prologue: Signal */
 1307               	/* frame size = 0 */
 1308 0026 E091 0000 		lds r30,EE_READY_handler+2
 1309 002a F091 0000 		lds r31,(EE_READY_handler+2)+1
 1310 002e 8091 0000 		lds r24,EE_READY_handler
 1311 0032 9091 0000 		lds r25,(EE_READY_handler)+1
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 24


 1312 0036 0995      		icall
 1313               	/* epilogue start */
 1314 0038 FF91      		pop r31
 1315 003a EF91      		pop r30
 1316 003c BF91      		pop r27
 1317 003e AF91      		pop r26
 1318 0040 9F91      		pop r25
 1319 0042 8F91      		pop r24
 1320 0044 7F91      		pop r23
 1321 0046 6F91      		pop r22
 1322 0048 5F91      		pop r21
 1323 004a 4F91      		pop r20
 1324 004c 3F91      		pop r19
 1325 004e 2F91      		pop r18
 1326 0050 0F90      		pop r0
 1327 0052 0BBE      		out 91-32,r0
 1328 0054 0F90      		pop r0
 1329 0056 0FBE      		out __SREG__,r0
 1330 0058 0F90      		pop r0
 1331 005a 1F90      		pop __zero_reg__
 1332 005c 1895      		reti
 1333               		.size	__vector_26, .-__vector_26
 1334               		.section	.text.__vector_27,"ax",@progbits
 1335               	.global	__vector_27
 1336               		.type	__vector_27, @function
 1337               	__vector_27:
 1338 0000 1F92      		push __zero_reg__
 1339 0002 0F92      		push r0
 1340 0004 0FB6      		in r0,__SREG__
 1341 0006 0F92      		push r0
 1342 0008 0BB6      		in r0,91-32
 1343 000a 0F92      		push r0
 1344 000c 1124      		clr __zero_reg__
 1345 000e 2F93      		push r18
 1346 0010 3F93      		push r19
 1347 0012 4F93      		push r20
 1348 0014 5F93      		push r21
 1349 0016 6F93      		push r22
 1350 0018 7F93      		push r23
 1351 001a 8F93      		push r24
 1352 001c 9F93      		push r25
 1353 001e AF93      		push r26
 1354 0020 BF93      		push r27
 1355 0022 EF93      		push r30
 1356 0024 FF93      		push r31
 1357               	/* prologue: Signal */
 1358               	/* frame size = 0 */
 1359 0026 E091 0000 		lds r30,TIMER3_CAPT_handler+2
 1360 002a F091 0000 		lds r31,(TIMER3_CAPT_handler+2)+1
 1361 002e 8091 0000 		lds r24,TIMER3_CAPT_handler
 1362 0032 9091 0000 		lds r25,(TIMER3_CAPT_handler)+1
 1363 0036 0995      		icall
 1364               	/* epilogue start */
 1365 0038 FF91      		pop r31
 1366 003a EF91      		pop r30
 1367 003c BF91      		pop r27
 1368 003e AF91      		pop r26
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 25


 1369 0040 9F91      		pop r25
 1370 0042 8F91      		pop r24
 1371 0044 7F91      		pop r23
 1372 0046 6F91      		pop r22
 1373 0048 5F91      		pop r21
 1374 004a 4F91      		pop r20
 1375 004c 3F91      		pop r19
 1376 004e 2F91      		pop r18
 1377 0050 0F90      		pop r0
 1378 0052 0BBE      		out 91-32,r0
 1379 0054 0F90      		pop r0
 1380 0056 0FBE      		out __SREG__,r0
 1381 0058 0F90      		pop r0
 1382 005a 1F90      		pop __zero_reg__
 1383 005c 1895      		reti
 1384               		.size	__vector_27, .-__vector_27
 1385               		.section	.text.__vector_28,"ax",@progbits
 1386               	.global	__vector_28
 1387               		.type	__vector_28, @function
 1388               	__vector_28:
 1389 0000 1F92      		push __zero_reg__
 1390 0002 0F92      		push r0
 1391 0004 0FB6      		in r0,__SREG__
 1392 0006 0F92      		push r0
 1393 0008 0BB6      		in r0,91-32
 1394 000a 0F92      		push r0
 1395 000c 1124      		clr __zero_reg__
 1396 000e 2F93      		push r18
 1397 0010 3F93      		push r19
 1398 0012 4F93      		push r20
 1399 0014 5F93      		push r21
 1400 0016 6F93      		push r22
 1401 0018 7F93      		push r23
 1402 001a 8F93      		push r24
 1403 001c 9F93      		push r25
 1404 001e AF93      		push r26
 1405 0020 BF93      		push r27
 1406 0022 EF93      		push r30
 1407 0024 FF93      		push r31
 1408               	/* prologue: Signal */
 1409               	/* frame size = 0 */
 1410 0026 E091 0000 		lds r30,TIMER3_COMPA_handler+2
 1411 002a F091 0000 		lds r31,(TIMER3_COMPA_handler+2)+1
 1412 002e 8091 0000 		lds r24,TIMER3_COMPA_handler
 1413 0032 9091 0000 		lds r25,(TIMER3_COMPA_handler)+1
 1414 0036 0995      		icall
 1415               	/* epilogue start */
 1416 0038 FF91      		pop r31
 1417 003a EF91      		pop r30
 1418 003c BF91      		pop r27
 1419 003e AF91      		pop r26
 1420 0040 9F91      		pop r25
 1421 0042 8F91      		pop r24
 1422 0044 7F91      		pop r23
 1423 0046 6F91      		pop r22
 1424 0048 5F91      		pop r21
 1425 004a 4F91      		pop r20
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 26


 1426 004c 3F91      		pop r19
 1427 004e 2F91      		pop r18
 1428 0050 0F90      		pop r0
 1429 0052 0BBE      		out 91-32,r0
 1430 0054 0F90      		pop r0
 1431 0056 0FBE      		out __SREG__,r0
 1432 0058 0F90      		pop r0
 1433 005a 1F90      		pop __zero_reg__
 1434 005c 1895      		reti
 1435               		.size	__vector_28, .-__vector_28
 1436               		.section	.text.__vector_29,"ax",@progbits
 1437               	.global	__vector_29
 1438               		.type	__vector_29, @function
 1439               	__vector_29:
 1440 0000 1F92      		push __zero_reg__
 1441 0002 0F92      		push r0
 1442 0004 0FB6      		in r0,__SREG__
 1443 0006 0F92      		push r0
 1444 0008 0BB6      		in r0,91-32
 1445 000a 0F92      		push r0
 1446 000c 1124      		clr __zero_reg__
 1447 000e 2F93      		push r18
 1448 0010 3F93      		push r19
 1449 0012 4F93      		push r20
 1450 0014 5F93      		push r21
 1451 0016 6F93      		push r22
 1452 0018 7F93      		push r23
 1453 001a 8F93      		push r24
 1454 001c 9F93      		push r25
 1455 001e AF93      		push r26
 1456 0020 BF93      		push r27
 1457 0022 EF93      		push r30
 1458 0024 FF93      		push r31
 1459               	/* prologue: Signal */
 1460               	/* frame size = 0 */
 1461 0026 E091 0000 		lds r30,TIMER3_COMPB_handler+2
 1462 002a F091 0000 		lds r31,(TIMER3_COMPB_handler+2)+1
 1463 002e 8091 0000 		lds r24,TIMER3_COMPB_handler
 1464 0032 9091 0000 		lds r25,(TIMER3_COMPB_handler)+1
 1465 0036 0995      		icall
 1466               	/* epilogue start */
 1467 0038 FF91      		pop r31
 1468 003a EF91      		pop r30
 1469 003c BF91      		pop r27
 1470 003e AF91      		pop r26
 1471 0040 9F91      		pop r25
 1472 0042 8F91      		pop r24
 1473 0044 7F91      		pop r23
 1474 0046 6F91      		pop r22
 1475 0048 5F91      		pop r21
 1476 004a 4F91      		pop r20
 1477 004c 3F91      		pop r19
 1478 004e 2F91      		pop r18
 1479 0050 0F90      		pop r0
 1480 0052 0BBE      		out 91-32,r0
 1481 0054 0F90      		pop r0
 1482 0056 0FBE      		out __SREG__,r0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 27


 1483 0058 0F90      		pop r0
 1484 005a 1F90      		pop __zero_reg__
 1485 005c 1895      		reti
 1486               		.size	__vector_29, .-__vector_29
 1487               		.section	.text.__vector_30,"ax",@progbits
 1488               	.global	__vector_30
 1489               		.type	__vector_30, @function
 1490               	__vector_30:
 1491 0000 1F92      		push __zero_reg__
 1492 0002 0F92      		push r0
 1493 0004 0FB6      		in r0,__SREG__
 1494 0006 0F92      		push r0
 1495 0008 0BB6      		in r0,91-32
 1496 000a 0F92      		push r0
 1497 000c 1124      		clr __zero_reg__
 1498 000e 2F93      		push r18
 1499 0010 3F93      		push r19
 1500 0012 4F93      		push r20
 1501 0014 5F93      		push r21
 1502 0016 6F93      		push r22
 1503 0018 7F93      		push r23
 1504 001a 8F93      		push r24
 1505 001c 9F93      		push r25
 1506 001e AF93      		push r26
 1507 0020 BF93      		push r27
 1508 0022 EF93      		push r30
 1509 0024 FF93      		push r31
 1510               	/* prologue: Signal */
 1511               	/* frame size = 0 */
 1512 0026 E091 0000 		lds r30,TIMER3_COMPC_handler+2
 1513 002a F091 0000 		lds r31,(TIMER3_COMPC_handler+2)+1
 1514 002e 8091 0000 		lds r24,TIMER3_COMPC_handler
 1515 0032 9091 0000 		lds r25,(TIMER3_COMPC_handler)+1
 1516 0036 0995      		icall
 1517               	/* epilogue start */
 1518 0038 FF91      		pop r31
 1519 003a EF91      		pop r30
 1520 003c BF91      		pop r27
 1521 003e AF91      		pop r26
 1522 0040 9F91      		pop r25
 1523 0042 8F91      		pop r24
 1524 0044 7F91      		pop r23
 1525 0046 6F91      		pop r22
 1526 0048 5F91      		pop r21
 1527 004a 4F91      		pop r20
 1528 004c 3F91      		pop r19
 1529 004e 2F91      		pop r18
 1530 0050 0F90      		pop r0
 1531 0052 0BBE      		out 91-32,r0
 1532 0054 0F90      		pop r0
 1533 0056 0FBE      		out __SREG__,r0
 1534 0058 0F90      		pop r0
 1535 005a 1F90      		pop __zero_reg__
 1536 005c 1895      		reti
 1537               		.size	__vector_30, .-__vector_30
 1538               		.section	.text.__vector_31,"ax",@progbits
 1539               	.global	__vector_31
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 28


 1540               		.type	__vector_31, @function
 1541               	__vector_31:
 1542 0000 1F92      		push __zero_reg__
 1543 0002 0F92      		push r0
 1544 0004 0FB6      		in r0,__SREG__
 1545 0006 0F92      		push r0
 1546 0008 0BB6      		in r0,91-32
 1547 000a 0F92      		push r0
 1548 000c 1124      		clr __zero_reg__
 1549 000e 2F93      		push r18
 1550 0010 3F93      		push r19
 1551 0012 4F93      		push r20
 1552 0014 5F93      		push r21
 1553 0016 6F93      		push r22
 1554 0018 7F93      		push r23
 1555 001a 8F93      		push r24
 1556 001c 9F93      		push r25
 1557 001e AF93      		push r26
 1558 0020 BF93      		push r27
 1559 0022 EF93      		push r30
 1560 0024 FF93      		push r31
 1561               	/* prologue: Signal */
 1562               	/* frame size = 0 */
 1563 0026 E091 0000 		lds r30,TIMER3_OVF_handler+2
 1564 002a F091 0000 		lds r31,(TIMER3_OVF_handler+2)+1
 1565 002e 8091 0000 		lds r24,TIMER3_OVF_handler
 1566 0032 9091 0000 		lds r25,(TIMER3_OVF_handler)+1
 1567 0036 0995      		icall
 1568               	/* epilogue start */
 1569 0038 FF91      		pop r31
 1570 003a EF91      		pop r30
 1571 003c BF91      		pop r27
 1572 003e AF91      		pop r26
 1573 0040 9F91      		pop r25
 1574 0042 8F91      		pop r24
 1575 0044 7F91      		pop r23
 1576 0046 6F91      		pop r22
 1577 0048 5F91      		pop r21
 1578 004a 4F91      		pop r20
 1579 004c 3F91      		pop r19
 1580 004e 2F91      		pop r18
 1581 0050 0F90      		pop r0
 1582 0052 0BBE      		out 91-32,r0
 1583 0054 0F90      		pop r0
 1584 0056 0FBE      		out __SREG__,r0
 1585 0058 0F90      		pop r0
 1586 005a 1F90      		pop __zero_reg__
 1587 005c 1895      		reti
 1588               		.size	__vector_31, .-__vector_31
 1589               		.section	.text.__vector_32,"ax",@progbits
 1590               	.global	__vector_32
 1591               		.type	__vector_32, @function
 1592               	__vector_32:
 1593 0000 1F92      		push __zero_reg__
 1594 0002 0F92      		push r0
 1595 0004 0FB6      		in r0,__SREG__
 1596 0006 0F92      		push r0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 29


 1597 0008 0BB6      		in r0,91-32
 1598 000a 0F92      		push r0
 1599 000c 1124      		clr __zero_reg__
 1600 000e 2F93      		push r18
 1601 0010 3F93      		push r19
 1602 0012 4F93      		push r20
 1603 0014 5F93      		push r21
 1604 0016 6F93      		push r22
 1605 0018 7F93      		push r23
 1606 001a 8F93      		push r24
 1607 001c 9F93      		push r25
 1608 001e AF93      		push r26
 1609 0020 BF93      		push r27
 1610 0022 EF93      		push r30
 1611 0024 FF93      		push r31
 1612               	/* prologue: Signal */
 1613               	/* frame size = 0 */
 1614 0026 E091 0000 		lds r30,USART1_RX_handler+2
 1615 002a F091 0000 		lds r31,(USART1_RX_handler+2)+1
 1616 002e 8091 0000 		lds r24,USART1_RX_handler
 1617 0032 9091 0000 		lds r25,(USART1_RX_handler)+1
 1618 0036 0995      		icall
 1619               	/* epilogue start */
 1620 0038 FF91      		pop r31
 1621 003a EF91      		pop r30
 1622 003c BF91      		pop r27
 1623 003e AF91      		pop r26
 1624 0040 9F91      		pop r25
 1625 0042 8F91      		pop r24
 1626 0044 7F91      		pop r23
 1627 0046 6F91      		pop r22
 1628 0048 5F91      		pop r21
 1629 004a 4F91      		pop r20
 1630 004c 3F91      		pop r19
 1631 004e 2F91      		pop r18
 1632 0050 0F90      		pop r0
 1633 0052 0BBE      		out 91-32,r0
 1634 0054 0F90      		pop r0
 1635 0056 0FBE      		out __SREG__,r0
 1636 0058 0F90      		pop r0
 1637 005a 1F90      		pop __zero_reg__
 1638 005c 1895      		reti
 1639               		.size	__vector_32, .-__vector_32
 1640               		.section	.text.__vector_33,"ax",@progbits
 1641               	.global	__vector_33
 1642               		.type	__vector_33, @function
 1643               	__vector_33:
 1644 0000 1F92      		push __zero_reg__
 1645 0002 0F92      		push r0
 1646 0004 0FB6      		in r0,__SREG__
 1647 0006 0F92      		push r0
 1648 0008 0BB6      		in r0,91-32
 1649 000a 0F92      		push r0
 1650 000c 1124      		clr __zero_reg__
 1651 000e 2F93      		push r18
 1652 0010 3F93      		push r19
 1653 0012 4F93      		push r20
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 30


 1654 0014 5F93      		push r21
 1655 0016 6F93      		push r22
 1656 0018 7F93      		push r23
 1657 001a 8F93      		push r24
 1658 001c 9F93      		push r25
 1659 001e AF93      		push r26
 1660 0020 BF93      		push r27
 1661 0022 EF93      		push r30
 1662 0024 FF93      		push r31
 1663               	/* prologue: Signal */
 1664               	/* frame size = 0 */
 1665 0026 E091 0000 		lds r30,USART1_UDRE_handler+2
 1666 002a F091 0000 		lds r31,(USART1_UDRE_handler+2)+1
 1667 002e 8091 0000 		lds r24,USART1_UDRE_handler
 1668 0032 9091 0000 		lds r25,(USART1_UDRE_handler)+1
 1669 0036 0995      		icall
 1670               	/* epilogue start */
 1671 0038 FF91      		pop r31
 1672 003a EF91      		pop r30
 1673 003c BF91      		pop r27
 1674 003e AF91      		pop r26
 1675 0040 9F91      		pop r25
 1676 0042 8F91      		pop r24
 1677 0044 7F91      		pop r23
 1678 0046 6F91      		pop r22
 1679 0048 5F91      		pop r21
 1680 004a 4F91      		pop r20
 1681 004c 3F91      		pop r19
 1682 004e 2F91      		pop r18
 1683 0050 0F90      		pop r0
 1684 0052 0BBE      		out 91-32,r0
 1685 0054 0F90      		pop r0
 1686 0056 0FBE      		out __SREG__,r0
 1687 0058 0F90      		pop r0
 1688 005a 1F90      		pop __zero_reg__
 1689 005c 1895      		reti
 1690               		.size	__vector_33, .-__vector_33
 1691               		.section	.text.__vector_34,"ax",@progbits
 1692               	.global	__vector_34
 1693               		.type	__vector_34, @function
 1694               	__vector_34:
 1695 0000 1F92      		push __zero_reg__
 1696 0002 0F92      		push r0
 1697 0004 0FB6      		in r0,__SREG__
 1698 0006 0F92      		push r0
 1699 0008 0BB6      		in r0,91-32
 1700 000a 0F92      		push r0
 1701 000c 1124      		clr __zero_reg__
 1702 000e 2F93      		push r18
 1703 0010 3F93      		push r19
 1704 0012 4F93      		push r20
 1705 0014 5F93      		push r21
 1706 0016 6F93      		push r22
 1707 0018 7F93      		push r23
 1708 001a 8F93      		push r24
 1709 001c 9F93      		push r25
 1710 001e AF93      		push r26
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 31


 1711 0020 BF93      		push r27
 1712 0022 EF93      		push r30
 1713 0024 FF93      		push r31
 1714               	/* prologue: Signal */
 1715               	/* frame size = 0 */
 1716 0026 E091 0000 		lds r30,USART1_TX_handler+2
 1717 002a F091 0000 		lds r31,(USART1_TX_handler+2)+1
 1718 002e 8091 0000 		lds r24,USART1_TX_handler
 1719 0032 9091 0000 		lds r25,(USART1_TX_handler)+1
 1720 0036 0995      		icall
 1721               	/* epilogue start */
 1722 0038 FF91      		pop r31
 1723 003a EF91      		pop r30
 1724 003c BF91      		pop r27
 1725 003e AF91      		pop r26
 1726 0040 9F91      		pop r25
 1727 0042 8F91      		pop r24
 1728 0044 7F91      		pop r23
 1729 0046 6F91      		pop r22
 1730 0048 5F91      		pop r21
 1731 004a 4F91      		pop r20
 1732 004c 3F91      		pop r19
 1733 004e 2F91      		pop r18
 1734 0050 0F90      		pop r0
 1735 0052 0BBE      		out 91-32,r0
 1736 0054 0F90      		pop r0
 1737 0056 0FBE      		out __SREG__,r0
 1738 0058 0F90      		pop r0
 1739 005a 1F90      		pop __zero_reg__
 1740 005c 1895      		reti
 1741               		.size	__vector_34, .-__vector_34
 1742               		.section	.text.__vector_35,"ax",@progbits
 1743               	.global	__vector_35
 1744               		.type	__vector_35, @function
 1745               	__vector_35:
 1746 0000 1F92      		push __zero_reg__
 1747 0002 0F92      		push r0
 1748 0004 0FB6      		in r0,__SREG__
 1749 0006 0F92      		push r0
 1750 0008 0BB6      		in r0,91-32
 1751 000a 0F92      		push r0
 1752 000c 1124      		clr __zero_reg__
 1753 000e 2F93      		push r18
 1754 0010 3F93      		push r19
 1755 0012 4F93      		push r20
 1756 0014 5F93      		push r21
 1757 0016 6F93      		push r22
 1758 0018 7F93      		push r23
 1759 001a 8F93      		push r24
 1760 001c 9F93      		push r25
 1761 001e AF93      		push r26
 1762 0020 BF93      		push r27
 1763 0022 EF93      		push r30
 1764 0024 FF93      		push r31
 1765               	/* prologue: Signal */
 1766               	/* frame size = 0 */
 1767 0026 E091 0000 		lds r30,TWI_handler+2
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 32


 1768 002a F091 0000 		lds r31,(TWI_handler+2)+1
 1769 002e 8091 0000 		lds r24,TWI_handler
 1770 0032 9091 0000 		lds r25,(TWI_handler)+1
 1771 0036 0995      		icall
 1772               	/* epilogue start */
 1773 0038 FF91      		pop r31
 1774 003a EF91      		pop r30
 1775 003c BF91      		pop r27
 1776 003e AF91      		pop r26
 1777 0040 9F91      		pop r25
 1778 0042 8F91      		pop r24
 1779 0044 7F91      		pop r23
 1780 0046 6F91      		pop r22
 1781 0048 5F91      		pop r21
 1782 004a 4F91      		pop r20
 1783 004c 3F91      		pop r19
 1784 004e 2F91      		pop r18
 1785 0050 0F90      		pop r0
 1786 0052 0BBE      		out 91-32,r0
 1787 0054 0F90      		pop r0
 1788 0056 0FBE      		out __SREG__,r0
 1789 0058 0F90      		pop r0
 1790 005a 1F90      		pop __zero_reg__
 1791 005c 1895      		reti
 1792               		.size	__vector_35, .-__vector_35
 1793               		.section	.text.__vector_36,"ax",@progbits
 1794               	.global	__vector_36
 1795               		.type	__vector_36, @function
 1796               	__vector_36:
 1797 0000 1F92      		push __zero_reg__
 1798 0002 0F92      		push r0
 1799 0004 0FB6      		in r0,__SREG__
 1800 0006 0F92      		push r0
 1801 0008 0BB6      		in r0,91-32
 1802 000a 0F92      		push r0
 1803 000c 1124      		clr __zero_reg__
 1804 000e 2F93      		push r18
 1805 0010 3F93      		push r19
 1806 0012 4F93      		push r20
 1807 0014 5F93      		push r21
 1808 0016 6F93      		push r22
 1809 0018 7F93      		push r23
 1810 001a 8F93      		push r24
 1811 001c 9F93      		push r25
 1812 001e AF93      		push r26
 1813 0020 BF93      		push r27
 1814 0022 EF93      		push r30
 1815 0024 FF93      		push r31
 1816               	/* prologue: Signal */
 1817               	/* frame size = 0 */
 1818 0026 E091 0000 		lds r30,SPM_READY_handler+2
 1819 002a F091 0000 		lds r31,(SPM_READY_handler+2)+1
 1820 002e 8091 0000 		lds r24,SPM_READY_handler
 1821 0032 9091 0000 		lds r25,(SPM_READY_handler)+1
 1822 0036 0995      		icall
 1823               	/* epilogue start */
 1824 0038 FF91      		pop r31
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 33


 1825 003a EF91      		pop r30
 1826 003c BF91      		pop r27
 1827 003e AF91      		pop r26
 1828 0040 9F91      		pop r25
 1829 0042 8F91      		pop r24
 1830 0044 7F91      		pop r23
 1831 0046 6F91      		pop r22
 1832 0048 5F91      		pop r21
 1833 004a 4F91      		pop r20
 1834 004c 3F91      		pop r19
 1835 004e 2F91      		pop r18
 1836 0050 0F90      		pop r0
 1837 0052 0BBE      		out 91-32,r0
 1838 0054 0F90      		pop r0
 1839 0056 0FBE      		out __SREG__,r0
 1840 0058 0F90      		pop r0
 1841 005a 1F90      		pop __zero_reg__
 1842 005c 1895      		reti
 1843               		.size	__vector_36, .-__vector_36
 1844               		.section	.text.__cxa_guard_acquire,"ax",@progbits
 1845               	.global	__cxa_guard_acquire
 1846               		.type	__cxa_guard_acquire, @function
 1847               	__cxa_guard_acquire:
 1848               	/* prologue: function */
 1849               	/* frame size = 0 */
 1850 0000 FC01      		movw r30,r24
 1851 0002 20E0      		ldi r18,lo8(0)
 1852 0004 30E0      		ldi r19,hi8(0)
 1853 0006 8081      		ld r24,Z
 1854 0008 8823      		tst r24
 1855 000a 01F4      		brne .L74
 1856 000c 21E0      		ldi r18,lo8(1)
 1857 000e 30E0      		ldi r19,hi8(1)
 1858               	.L74:
 1859 0010 C901      		movw r24,r18
 1860               	/* epilogue start */
 1861 0012 0895      		ret
 1862               		.size	__cxa_guard_acquire, .-__cxa_guard_acquire
 1863               		.section	.text.__cxa_guard_release,"ax",@progbits
 1864               	.global	__cxa_guard_release
 1865               		.type	__cxa_guard_release, @function
 1866               	__cxa_guard_release:
 1867               	/* prologue: function */
 1868               	/* frame size = 0 */
 1869 0000 FC01      		movw r30,r24
 1870 0002 81E0      		ldi r24,lo8(1)
 1871 0004 8083      		st Z,r24
 1872               	/* epilogue start */
 1873 0006 0895      		ret
 1874               		.size	__cxa_guard_release, .-__cxa_guard_release
 1875               		.section	.text.__cxa_guard_abort,"ax",@progbits
 1876               	.global	__cxa_guard_abort
 1877               		.type	__cxa_guard_abort, @function
 1878               	__cxa_guard_abort:
 1879               	/* prologue: function */
 1880               	/* frame size = 0 */
 1881               	/* epilogue start */
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 34


 1882 0000 0895      		ret
 1883               		.size	__cxa_guard_abort, .-__cxa_guard_abort
 1884               		.section	.text.__cxa_pure_virtual,"ax",@progbits
 1885               	.global	__cxa_pure_virtual
 1886               		.type	__cxa_pure_virtual, @function
 1887               	__cxa_pure_virtual:
 1888               	/* prologue: function */
 1889               	/* frame size = 0 */
 1890               	/* epilogue start */
 1891 0000 0895      		ret
 1892               		.size	__cxa_pure_virtual, .-__cxa_pure_virtual
 1893               		.section	.text._Z3powmm,"ax",@progbits
 1894               	.global	_Z3powmm
 1895               		.type	_Z3powmm, @function
 1896               	_Z3powmm:
 1897 0000 2F92      		push r2
 1898 0002 3F92      		push r3
 1899 0004 4F92      		push r4
 1900 0006 5F92      		push r5
 1901 0008 6F92      		push r6
 1902 000a 7F92      		push r7
 1903 000c 8F92      		push r8
 1904 000e 9F92      		push r9
 1905 0010 AF92      		push r10
 1906 0012 BF92      		push r11
 1907 0014 CF92      		push r12
 1908 0016 DF92      		push r13
 1909 0018 EF92      		push r14
 1910 001a FF92      		push r15
 1911 001c 0F93      		push r16
 1912 001e 1F93      		push r17
 1913 0020 DF93      		push r29
 1914 0022 CF93      		push r28
 1915 0024 00D0      		rcall .
 1916 0026 00D0      		rcall .
 1917 0028 CDB7      		in r28,__SP_L__
 1918 002a DEB7      		in r29,__SP_H__
 1919               	/* prologue: function */
 1920               	/* frame size = 4 */
 1921 002c 2983      		std Y+1,r18
 1922 002e 3A83      		std Y+2,r19
 1923 0030 4B83      		std Y+3,r20
 1924 0032 5C83      		std Y+4,r21
 1925 0034 2130      		cpi r18,lo8(1)
 1926 0036 3105      		cpc r19,__zero_reg__
 1927 0038 4105      		cpc r20,__zero_reg__
 1928 003a 5105      		cpc r21,__zero_reg__
 1929 003c 01F4      		brne .+2
 1930 003e 00C0      		rjmp .L88
 1931 0040 21E0      		ldi r18,lo8(1)
 1932 0042 30E0      		ldi r19,lo8(0)
 1933 0044 40E0      		ldi r20,lo8(0)
 1934 0046 50E0      		ldi r21,lo8(0)
 1935 0048 10E0      		ldi r17,lo8(0)
 1936 004a B0E0      		ldi r27,lo8(0)
 1937 004c A0E0      		ldi r26,lo8(0)
 1938 004e 00E0      		ldi r16,lo8(0)
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 35


 1939 0050 1B01      		movw r2,r22
 1940 0052 2C01      		movw r4,r24
 1941 0054 6981      		ldd r22,Y+1
 1942 0056 7A81      		ldd r23,Y+2
 1943 0058 8B81      		ldd r24,Y+3
 1944 005a 9C81      		ldd r25,Y+4
 1945               	.L86:
 1946 005c 6150      		subi r22,lo8(-(-1))
 1947 005e 7040      		sbci r23,hi8(-(-1))
 1948 0060 8040      		sbci r24,hlo8(-(-1))
 1949 0062 9040      		sbci r25,hhi8(-(-1))
 1950 0064 6983      		std Y+1,r22
 1951 0066 7A83      		std Y+2,r23
 1952 0068 8B83      		std Y+3,r24
 1953 006a 9C83      		std Y+4,r25
 1954 006c 5101      		movw r10,r2
 1955 006e 6201      		movw r12,r4
 1956 0070 612F      		mov r22,r17
 1957 0072 7B2F      		mov r23,r27
 1958 0074 8A2F      		mov r24,r26
 1959 0076 902F      		mov r25,r16
 1960 0078 EE24      		clr r14
 1961 007a FF24      		clr r15
 1962 007c 00E0      		ldi r16,lo8(0)
 1963 007e 10E0      		ldi r17,lo8(0)
 1964 0080 0E94 0000 		call __muldi3
 1965 0084 162F      		mov r17,r22
 1966 0086 B72F      		mov r27,r23
 1967 0088 A82F      		mov r26,r24
 1968 008a 092F      		mov r16,r25
 1969 008c 6981      		ldd r22,Y+1
 1970 008e 7A81      		ldd r23,Y+2
 1971 0090 8B81      		ldd r24,Y+3
 1972 0092 9C81      		ldd r25,Y+4
 1973 0094 6130      		cpi r22,lo8(1)
 1974 0096 7105      		cpc r23,__zero_reg__
 1975 0098 8105      		cpc r24,__zero_reg__
 1976 009a 9105      		cpc r25,__zero_reg__
 1977 009c 01F4      		brne .L86
 1978               	.L85:
 1979 009e 5101      		movw r10,r2
 1980 00a0 6201      		movw r12,r4
 1981 00a2 612F      		mov r22,r17
 1982 00a4 7B2F      		mov r23,r27
 1983 00a6 8A2F      		mov r24,r26
 1984 00a8 902F      		mov r25,r16
 1985 00aa EE24      		clr r14
 1986 00ac FF24      		clr r15
 1987 00ae 00E0      		ldi r16,lo8(0)
 1988 00b0 10E0      		ldi r17,lo8(0)
 1989 00b2 0E94 0000 		call __muldi3
 1990               	/* epilogue start */
 1991 00b6 0F90      		pop __tmp_reg__
 1992 00b8 0F90      		pop __tmp_reg__
 1993 00ba 0F90      		pop __tmp_reg__
 1994 00bc 0F90      		pop __tmp_reg__
 1995 00be CF91      		pop r28
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 36


 1996 00c0 DF91      		pop r29
 1997 00c2 1F91      		pop r17
 1998 00c4 0F91      		pop r16
 1999 00c6 FF90      		pop r15
 2000 00c8 EF90      		pop r14
 2001 00ca DF90      		pop r13
 2002 00cc CF90      		pop r12
 2003 00ce BF90      		pop r11
 2004 00d0 AF90      		pop r10
 2005 00d2 9F90      		pop r9
 2006 00d4 8F90      		pop r8
 2007 00d6 7F90      		pop r7
 2008 00d8 6F90      		pop r6
 2009 00da 5F90      		pop r5
 2010 00dc 4F90      		pop r4
 2011 00de 3F90      		pop r3
 2012 00e0 2F90      		pop r2
 2013 00e2 0895      		ret
 2014               	.L88:
 2015 00e4 21E0      		ldi r18,lo8(1)
 2016 00e6 30E0      		ldi r19,lo8(0)
 2017 00e8 40E0      		ldi r20,lo8(0)
 2018 00ea 50E0      		ldi r21,lo8(0)
 2019 00ec 10E0      		ldi r17,lo8(0)
 2020 00ee B0E0      		ldi r27,lo8(0)
 2021 00f0 A0E0      		ldi r26,lo8(0)
 2022 00f2 00E0      		ldi r16,lo8(0)
 2023 00f4 1B01      		movw r2,r22
 2024 00f6 2C01      		movw r4,r24
 2025 00f8 00C0      		rjmp .L85
 2026               		.size	_Z3powmm, .-_Z3powmm
 2027               		.section	.init5,"ax",@progbits
 2028               	.global	_Z4Initv
 2029               		.type	_Z4Initv, @function
 2030               	_Z4Initv:
 2031               	/* prologue: naked */
 2032               	/* frame size = 0 */
 2033 0000 3C9A      		sbi 39-32,4
 2034 0002 3D9A      		sbi 39-32,5
 2035 0004 449A      		sbi 40-32,4
 2036 0006 459A      		sbi 40-32,5
 2037               	/* epilogue start */
 2038               		.size	_Z4Initv, .-_Z4Initv
 2039               		.section	.text.main,"ax",@progbits
 2040               	.global	main
 2041               		.type	main, @function
 2042               	main:
 2043               	/* prologue: function */
 2044               	/* frame size = 0 */
 2045 0000 8AE0      		ldi r24,lo8(10)
 2046 0002 8093 0000 		sts com+3,r24
 2047               	/* #APP */
 2048               	 ;  239 "../main.cpp" 1
 2049 0006 7894      		sei
 2050               	 ;  0 "" 2
 2051               	/* #NOAPP */
 2052 0008 9091 0000 		lds r25,dispatcher+1536
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 37


 2053               	.L95:
 2054 000c 8091 0000 		lds r24,dispatcher+1537
 2055 0010 9817      		cp r25,r24
 2056 0012 01F0      		breq .L92
 2057               	.L96:
 2058 0014 892F      		mov r24,r25
 2059 0016 90E0      		ldi r25,lo8(0)
 2060 0018 FC01      		movw r30,r24
 2061 001a EE0F      		lsl r30
 2062 001c FF1F      		rol r31
 2063 001e E80F      		add r30,r24
 2064 0020 F91F      		adc r31,r25
 2065 0022 EE0F      		lsl r30
 2066 0024 FF1F      		rol r31
 2067 0026 DF01      		movw r26,r30
 2068 0028 A050      		subi r26,lo8(-(dispatcher))
 2069 002a B040      		sbci r27,hi8(-(dispatcher))
 2070 002c 1496      		adiw r26,4
 2071 002e 6D91      		ld r22,X+
 2072 0030 7C91      		ld r23,X
 2073 0032 1597      		sbiw r26,4+1
 2074 0034 E050      		subi r30,lo8(-(dispatcher+2))
 2075 0036 F040      		sbci r31,hi8(-(dispatcher+2))
 2076 0038 0190      		ld __tmp_reg__,Z+
 2077 003a F081      		ld r31,Z
 2078 003c E02D      		mov r30,__tmp_reg__
 2079 003e 8D91      		ld r24,X+
 2080 0040 9C91      		ld r25,X
 2081 0042 0995      		icall
 2082 0044 8091 0000 		lds r24,dispatcher+1536
 2083 0048 8F5F      		subi r24,lo8(-(1))
 2084 004a 8093 0000 		sts dispatcher+1536,r24
 2085 004e 982F      		mov r25,r24
 2086 0050 8091 0000 		lds r24,dispatcher+1537
 2087 0054 9817      		cp r25,r24
 2088 0056 01F4      		brne .L96
 2089               	.L92:
 2090               	/* #APP */
 2091               	 ;  89 "D:\Programming\avr-cpp-libs\my/cpp/dispatcher.h" 1
 2092 0058 0000      		nop
 2093               	 ;  0 "" 2
 2094               	/* #NOAPP */
 2095 005a 9091 0000 		lds r25,dispatcher+1536
 2096 005e 00C0      		rjmp .L95
 2097               		.size	main, .-main
 2098               		.section	.text._ZN6srutil8DelegateIFvvEE14empty_functionEPv,"ax",@progbits
 2099               		.weak	_ZN6srutil8DelegateIFvvEE14empty_functionEPv
 2100               		.type	_ZN6srutil8DelegateIFvvEE14empty_functionEPv, @function
 2101               	_ZN6srutil8DelegateIFvvEE14empty_functionEPv:
 2102               	/* prologue: function */
 2103               	/* frame size = 0 */
 2104               	/* epilogue start */
 2105 0000 0895      		ret
 2106               		.size	_ZN6srutil8DelegateIFvvEE14empty_functionEPv, .-_ZN6srutil8DelegateIFvvEE14empty_functionEPv
 2107               		.section	.text._ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2108               		.weak	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
 2109               		.type	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 38


 2110               	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8RegisterIh
 2111 0000 DF93      		push r29
 2112 0002 CF93      		push r28
 2113 0004 00D0      		rcall .
 2114 0006 00D0      		rcall .
 2115 0008 CDB7      		in r28,__SP_L__
 2116 000a DEB7      		in r29,__SP_H__
 2117               	/* prologue: function */
 2118               	/* frame size = 4 */
 2119 000c DC01      		movw r26,r24
 2120 000e 8091 C800 		lds r24,200
 2121 0012 2091 C900 		lds r18,201
 2122 0016 2695      		lsr r18
 2123 0018 2170      		andi r18,lo8(1)
 2124 001a 9091 CE00 		lds r25,206
 2125 001e 9983      		std Y+1,r25
 2126 0020 8871      		andi r24,lo8(24)
 2127 0022 01F4      		brne .L100
 2128 0024 392F      		mov r19,r25
 2129 0026 3295      		swap r19
 2130 0028 3F70      		andi r19,lo8(15)
 2131 002a E22F      		mov r30,r18
 2132 002c F0E0      		ldi r31,lo8(0)
 2133 002e E295      		swap r30
 2134 0030 F295      		swap r31
 2135 0032 F07F      		andi r31,0xf0
 2136 0034 FE27      		eor r31,r30
 2137 0036 E07F      		andi r30,0xf0
 2138 0038 FE27      		eor r31,r30
 2139 003a E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2140 003c F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2141 003e E30F      		add r30,r19
 2142 0040 F11D      		adc r31,__zero_reg__
 2143 0042 8081      		ld r24,Z
 2144 0044 8917      		cp r24,r25
 2145 0046 01F0      		breq .L124
 2146               	.L100:
 2147 0048 8091 C800 		lds r24,200
 2148 004c 8160      		ori r24,lo8(1)
 2149 004e 8093 C800 		sts 200,r24
 2150 0052 5C9A      		sbi 43-32,4
 2151 0054 8091 C900 		lds r24,201
 2152 0058 8061      		ori r24,lo8(16)
 2153 005a 8093 C900 		sts 201,r24
 2154 005e 1496      		adiw r26,4
 2155 0060 1C92      		st X,__zero_reg__
 2156               	.L114:
 2157 0062 8091 C900 		lds r24,201
 2158 0066 8068      		ori r24,lo8(-128)
 2159 0068 8093 C900 		sts 201,r24
 2160               	.L121:
 2161               	/* epilogue start */
 2162 006c 0F90      		pop __tmp_reg__
 2163 006e 0F90      		pop __tmp_reg__
 2164 0070 0F90      		pop __tmp_reg__
 2165 0072 0F90      		pop __tmp_reg__
 2166 0074 CF91      		pop r28
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 39


 2167 0076 DF91      		pop r29
 2168 0078 0895      		ret
 2169               	.L124:
 2170 007a 8091 C900 		lds r24,201
 2171 007e 8F77      		andi r24,lo8(127)
 2172 0080 8093 C900 		sts 201,r24
 2173               	/* #APP */
 2174               	 ;  278 "../SautCom.h" 1
 2175 0084 7894      		sei
 2176               	 ;  0 "" 2
 2177               	/* #NOAPP */
 2178 0086 2130      		cpi r18,lo8(1)
 2179 0088 01F0      		breq .+2
 2180 008a 00C0      		rjmp .L125
 2181 008c 232F      		mov r18,r19
 2182 008e 220F      		lsl r18
 2183 0090 220F      		lsl r18
 2184 0092 220F      		lsl r18
 2185 0094 00C0      		rjmp .L122
 2186               	.L127:
 2187 0096 1796      		adiw r26,7
 2188 0098 8C91      		ld r24,X
 2189 009a 1797      		sbiw r26,7
 2190 009c 8770      		andi r24,lo8(7)
 2191 009e 822B      		or r24,r18
 2192 00a0 1796      		adiw r26,7
 2193 00a2 8C93      		st X,r24
 2194 00a4 1797      		sbiw r26,7
 2195 00a6 8091 C800 		lds r24,200
 2196 00aa 8E7F      		andi r24,lo8(-2)
 2197 00ac 8093 C800 		sts 200,r24
 2198 00b0 1496      		adiw r26,4
 2199 00b2 9C91      		ld r25,X
 2200 00b4 1497      		sbiw r26,4
 2201               	.L108:
 2202 00b6 9230      		cpi r25,lo8(2)
 2203 00b8 01F4      		brne .+2
 2204 00ba 00C0      		rjmp .L103
 2205 00bc 9330      		cpi r25,lo8(3)
 2206 00be 01F4      		brne .+2
 2207 00c0 00C0      		rjmp .L126
 2208 00c2 9430      		cpi r25,lo8(4)
 2209 00c4 01F4      		brne .L106
 2210 00c6 1496      		adiw r26,4
 2211 00c8 1C92      		st X,__zero_reg__
 2212 00ca 1497      		sbiw r26,4
 2213               	.L122:
 2214 00cc 1496      		adiw r26,4
 2215 00ce 9C91      		ld r25,X
 2216 00d0 1497      		sbiw r26,4
 2217 00d2 9923      		tst r25
 2218 00d4 01F0      		breq .L127
 2219 00d6 9130      		cpi r25,lo8(1)
 2220 00d8 01F4      		brne .L108
 2221 00da 1796      		adiw r26,7
 2222 00dc 8C91      		ld r24,X
 2223 00de 1797      		sbiw r26,7
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 40


 2224 00e0 8068      		ori r24,lo8(-128)
 2225 00e2 1796      		adiw r26,7
 2226 00e4 8C93      		st X,r24
 2227 00e6 1797      		sbiw r26,7
 2228               	.L106:
 2229 00e8 9530      		cpi r25,lo8(5)
 2230 00ea 01F4      		brne .+2
 2231 00ec 00C0      		rjmp .L128
 2232 00ee 9630      		cpi r25,lo8(6)
 2233 00f0 01F4      		brne .+2
 2234 00f2 00C0      		rjmp .L129
 2235 00f4 9730      		cpi r25,lo8(7)
 2236 00f6 01F4      		brne .+2
 2237 00f8 00C0      		rjmp .L130
 2238               	.L112:
 2239 00fa 892F      		mov r24,r25
 2240 00fc 8F5F      		subi r24,lo8(-(1))
 2241 00fe 1496      		adiw r26,4
 2242 0100 8C93      		st X,r24
 2243 0102 1497      		sbiw r26,4
 2244 0104 8830      		cpi r24,lo8(8)
 2245 0106 01F0      		breq .+2
 2246 0108 00C0      		rjmp .L114
 2247 010a 8091 C800 		lds r24,200
 2248 010e 8160      		ori r24,lo8(1)
 2249 0110 8093 C800 		sts 200,r24
 2250 0114 5C9A      		sbi 43-32,4
 2251 0116 8091 C900 		lds r24,201
 2252 011a 8061      		ori r24,lo8(16)
 2253 011c 8093 C900 		sts 201,r24
 2254 0120 1496      		adiw r26,4
 2255 0122 1C92      		st X,__zero_reg__
 2256 0124 1497      		sbiw r26,4
 2257               	/* #APP */
 2258               	 ;  393 "../SautCom.h" 1
 2259 0126 F894      		cli
 2260               	 ;  0 "" 2
 2261               	/* #NOAPP */
 2262 0128 8091 C900 		lds r24,201
 2263 012c 8068      		ori r24,lo8(-128)
 2264 012e 8093 C900 		sts 201,r24
 2265 0132 1796      		adiw r26,7
 2266 0134 EC91      		ld r30,X
 2267 0136 1797      		sbiw r26,7
 2268 0138 F0E0      		ldi r31,lo8(0)
 2269 013a E050      		subi r30,lo8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2270 013c F040      		sbci r31,hi8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2271               	/* #APP */
 2272               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 2273 013e E491      		lpm r30, Z
 2274               		
 2275               	 ;  0 "" 2
 2276               	/* #NOAPP */
 2277 0140 EF77      		andi r30,lo8(127)
 2278 0142 2E2F      		mov r18,r30
 2279 0144 30E0      		ldi r19,lo8(0)
 2280 0146 1596      		adiw r26,5
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 41


 2281 0148 8D91      		ld r24,X+
 2282 014a 9C91      		ld r25,X
 2283 014c 1697      		sbiw r26,5+1
 2284 014e 9C83      		std Y+4,r25
 2285 0150 8B83      		std Y+3,r24
 2286 0152 F901      		movw r30,r18
 2287 0154 EE0F      		lsl r30
 2288 0156 FF1F      		rol r31
 2289 0158 E050      		subi r30,lo8(-(data+5))
 2290 015a F040      		sbci r31,hi8(-(data+5))
 2291 015c 9183      		std Z+1,r25
 2292 015e 8083      		st Z,r24
 2293 0160 8091 0000 		lds r24,data+28
 2294 0164 80FD      		sbrc r24,0
 2295 0166 00C0      		rjmp .L121
 2296 0168 8091 0000 		lds r24,data+28
 2297 016c 8260      		ori r24,lo8(2)
 2298 016e 8093 0000 		sts data+28,r24
 2299               	/* #APP */
 2300               	 ;  175 "../SautDat.h" 1
 2301 0172 7894      		sei
 2302               	 ;  0 "" 2
 2303               	/* #NOAPP */
 2304 0174 2050      		subi r18,lo8(-(_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE))
 2305 0176 3040      		sbci r19,hi8(-(_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE))
 2306 0178 F901      		movw r30,r18
 2307               	/* #APP */
 2308               	 ;  248 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 2309 017a 8491      		lpm r24, Z
 2310               		
 2311               	 ;  0 "" 2
 2312               	/* #NOAPP */
 2313 017c E82F      		mov r30,r24
 2314 017e F0E0      		ldi r31,lo8(0)
 2315 0180 EE0F      		lsl r30
 2316 0182 FF1F      		rol r31
 2317 0184 EE0F      		lsl r30
 2318 0186 FF1F      		rol r31
 2319 0188 DF01      		movw r26,r30
 2320 018a A050      		subi r26,lo8(-(data+24))
 2321 018c B040      		sbci r27,hi8(-(data+24))
 2322 018e E050      		subi r30,lo8(-(data))
 2323 0190 F040      		sbci r31,hi8(-(data))
 2324 0192 028C      		ldd __tmp_reg__,Z+26
 2325 0194 F38D      		ldd r31,Z+27
 2326 0196 E02D      		mov r30,__tmp_reg__
 2327 0198 8D91      		ld r24,X+
 2328 019a 9C91      		ld r25,X
 2329 019c 0995      		icall
 2330 019e 8091 0000 		lds r24,data+28
 2331 01a2 8D7F      		andi r24,lo8(-3)
 2332 01a4 8093 0000 		sts data+28,r24
 2333 01a8 00C0      		rjmp .L121
 2334               	.L128:
 2335 01aa 1696      		adiw r26,6
 2336 01ac 8C91      		ld r24,X
 2337 01ae 1697      		sbiw r26,6
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 42


 2338 01b0 807F      		andi r24,lo8(-16)
 2339 01b2 832B      		or r24,r19
 2340 01b4 1696      		adiw r26,6
 2341 01b6 8C93      		st X,r24
 2342 01b8 1697      		sbiw r26,6
 2343 01ba 00C0      		rjmp .L112
 2344               	.L125:
 2345 01bc 1496      		adiw r26,4
 2346 01be 9C91      		ld r25,X
 2347 01c0 1497      		sbiw r26,4
 2348 01c2 9923      		tst r25
 2349 01c4 01F4      		brne .+2
 2350 01c6 00C0      		rjmp .L100
 2351 01c8 9130      		cpi r25,lo8(1)
 2352 01ca 01F0      		breq .L102
 2353 01cc 9230      		cpi r25,lo8(2)
 2354 01ce 01F0      		breq .L103
 2355 01d0 9330      		cpi r25,lo8(3)
 2356 01d2 01F4      		brne .+2
 2357 01d4 00C0      		rjmp .L104
 2358 01d6 9430      		cpi r25,lo8(4)
 2359 01d8 01F0      		breq .+2
 2360 01da 00C0      		rjmp .L106
 2361 01dc 1696      		adiw r26,5+1
 2362 01de 1C92      		st X,__zero_reg__
 2363 01e0 1E92      		st -X,__zero_reg__
 2364 01e2 1597      		sbiw r26,5
 2365 01e4 3295      		swap r19
 2366 01e6 307F      		andi r19,lo8(-16)
 2367 01e8 1696      		adiw r26,6
 2368 01ea 8C91      		ld r24,X
 2369 01ec 1697      		sbiw r26,6
 2370 01ee 8F70      		andi r24,lo8(15)
 2371 01f0 832B      		or r24,r19
 2372 01f2 1696      		adiw r26,6
 2373 01f4 8C93      		st X,r24
 2374 01f6 1697      		sbiw r26,6
 2375 01f8 94E0      		ldi r25,lo8(4)
 2376 01fa 00C0      		rjmp .L112
 2377               	.L102:
 2378 01fc 82E0      		ldi r24,lo8(2)
 2379 01fe 1496      		adiw r26,4
 2380 0200 8C93      		st X,r24
 2381 0202 1497      		sbiw r26,4
 2382               	.L103:
 2383               	/* #APP */
 2384               	 ;  301 "../SautCom.h" 1
 2385 0204 F894      		cli
 2386               	 ;  0 "" 2
 2387               	 ;  302 "../SautCom.h" 1
 2388 0206 F894      		cli
 2389               	 ;  0 "" 2
 2390               	/* #NOAPP */
 2391 0208 3A83      		std Y+2,r19
 2392               	/* #APP */
 2393               	 ;  311 "../SautCom.h" 1
 2394 020a F894      		cli
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 43


 2395               	 ;  0 "" 2
 2396               	 ;  312 "../SautCom.h" 1
 2397 020c F894      		cli
 2398               	 ;  0 "" 2
 2399               	/* #NOAPP */
 2400 020e 93B1      		in r25,35-32
 2401 0210 2A81      		ldd r18,Y+2
 2402 0212 822F      		mov r24,r18
 2403 0214 8695      		lsr r24
 2404 0216 8695      		lsr r24
 2405 0218 8170      		andi r24,lo8(1)
 2406 021a 991F      		rol r25
 2407 021c 9927      		clr r25
 2408 021e 991F      		rol r25
 2409 0220 8917      		cp r24,r25
 2410 0222 01F0      		breq .+2
 2411 0224 00C0      		rjmp .L100
 2412 0226 822F      		mov r24,r18
 2413 0228 8695      		lsr r24
 2414 022a 8470      		andi r24,lo8(4)
 2415 022c 1796      		adiw r26,7
 2416 022e 9C91      		ld r25,X
 2417 0230 1797      		sbiw r26,7
 2418 0232 987F      		andi r25,lo8(-8)
 2419 0234 2370      		andi r18,lo8(3)
 2420 0236 982B      		or r25,r24
 2421 0238 922B      		or r25,r18
 2422 023a 1796      		adiw r26,7
 2423 023c 9C93      		st X,r25
 2424 023e 1797      		sbiw r26,7
 2425 0240 1296      		adiw r26,2
 2426 0242 8C91      		ld r24,X
 2427 0244 1297      		sbiw r26,2
 2428 0246 8823      		tst r24
 2429 0248 01F0      		breq .+2
 2430 024a 00C0      		rjmp .L123
 2431 024c 8091 C900 		lds r24,201
 2432 0250 8F7E      		andi r24,lo8(-17)
 2433 0252 8093 C900 		sts 201,r24
 2434 0256 5C98      		cbi 43-32,4
 2435 0258 1396      		adiw r26,3
 2436 025a EC91      		ld r30,X
 2437 025c 1397      		sbiw r26,3
 2438 025e F0E0      		ldi r31,lo8(0)
 2439 0260 EF70      		andi r30,lo8(15)
 2440 0262 F070      		andi r31,hi8(15)
 2441 0264 E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2442 0266 F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2443 0268 8081      		ld r24,Z
 2444 026a 8093 CE00 		sts 206,r24
 2445 026e 83E0      		ldi r24,lo8(3)
 2446 0270 1496      		adiw r26,4
 2447 0272 8C93      		st X,r24
 2448 0274 1497      		sbiw r26,4
 2449 0276 1796      		adiw r26,7
 2450 0278 EC91      		ld r30,X
 2451 027a 1797      		sbiw r26,7
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 44


 2452 027c F0E0      		ldi r31,lo8(0)
 2453 027e E050      		subi r30,lo8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2454 0280 F040      		sbci r31,hi8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2455               	/* #APP */
 2456               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 2457 0282 E491      		lpm r30, Z
 2458               		
 2459               	 ;  0 "" 2
 2460               	/* #NOAPP */
 2461 0284 E7FD      		sbrc r30,7
 2462 0286 00C0      		rjmp .L113
 2463 0288 84E0      		ldi r24,lo8(4)
 2464 028a 1496      		adiw r26,4
 2465 028c 8C93      		st X,r24
 2466 028e 00C0      		rjmp .L114
 2467               	.L126:
 2468 0290 1296      		adiw r26,2
 2469 0292 8C91      		ld r24,X
 2470 0294 1297      		sbiw r26,2
 2471 0296 8823      		tst r24
 2472 0298 01F4      		brne .+2
 2473 029a 00C0      		rjmp .L100
 2474 029c 1296      		adiw r26,2
 2475 029e 8C91      		ld r24,X
 2476 02a0 1297      		sbiw r26,2
 2477 02a2 8F5F      		subi r24,lo8(-(1))
 2478 02a4 1296      		adiw r26,2
 2479 02a6 8C93      		st X,r24
 2480 02a8 1297      		sbiw r26,2
 2481 02aa 1296      		adiw r26,2
 2482 02ac 8C91      		ld r24,X
 2483 02ae 1297      		sbiw r26,2
 2484 02b0 8530      		cpi r24,lo8(5)
 2485 02b2 01F0      		breq .+2
 2486 02b4 00C0      		rjmp .L100
 2487 02b6 1296      		adiw r26,2
 2488 02b8 1C92      		st X,__zero_reg__
 2489 02ba 1297      		sbiw r26,2
 2490 02bc 00C0      		rjmp .L100
 2491               	.L104:
 2492 02be 1796      		adiw r26,7
 2493 02c0 EC91      		ld r30,X
 2494 02c2 1797      		sbiw r26,7
 2495 02c4 F0E0      		ldi r31,lo8(0)
 2496 02c6 E050      		subi r30,lo8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2497 02c8 F040      		sbci r31,hi8(-(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13
 2498               	/* #APP */
 2499               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 2500 02ca E491      		lpm r30, Z
 2501               		
 2502               	 ;  0 "" 2
 2503               	/* #NOAPP */
 2504 02cc E7FD      		sbrc r30,7
 2505 02ce 00C0      		rjmp .L116
 2506 02d0 93E0      		ldi r25,lo8(3)
 2507 02d2 00C0      		rjmp .L112
 2508               	.L129:
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 45


 2509 02d4 3295      		swap r19
 2510 02d6 307F      		andi r19,lo8(-16)
 2511 02d8 1596      		adiw r26,5
 2512 02da 8C91      		ld r24,X
 2513 02dc 1597      		sbiw r26,5
 2514 02de 8F70      		andi r24,lo8(15)
 2515 02e0 832B      		or r24,r19
 2516 02e2 1596      		adiw r26,5
 2517 02e4 8C93      		st X,r24
 2518 02e6 1597      		sbiw r26,5
 2519 02e8 00C0      		rjmp .L112
 2520               	.L113:
 2521 02ea 9091 0000 		lds r25,data+28
 2522 02ee 9695      		lsr r25
 2523 02f0 9170      		andi r25,lo8(1)
 2524 02f2 8091 0000 		lds r24,data+28
 2525 02f6 8E7F      		andi r24,lo8(-2)
 2526 02f8 892B      		or r24,r25
 2527 02fa 8093 0000 		sts data+28,r24
 2528 02fe F0E0      		ldi r31,lo8(0)
 2529 0300 EF77      		andi r30,lo8(127)
 2530 0302 F070      		andi r31,hi8(127)
 2531 0304 2FB7      		in r18,__SREG__
 2532               	/* #APP */
 2533               	 ;  9 "D:\Programming\avr-cpp-libs\foreign/cpp/Chizhov/AVR/atomic.h" 1
 2534 0306 F894      		cli
 2535               	 ;  0 "" 2
 2536               	/* #NOAPP */
 2537 0308 EE0F      		lsl r30
 2538 030a FF1F      		rol r31
 2539 030c E050      		subi r30,lo8(-(data))
 2540 030e F040      		sbci r31,hi8(-(data))
 2541 0310 8581      		ldd r24,Z+5
 2542 0312 9681      		ldd r25,Z+6
 2543 0314 2FBF      		out __SREG__,r18
 2544 0316 1696      		adiw r26,5+1
 2545 0318 9C93      		st X,r25
 2546 031a 8E93      		st -X,r24
 2547 031c 1597      		sbiw r26,5
 2548 031e 1196      		adiw r26,1
 2549 0320 9C93      		st X,r25
 2550 0322 8E93      		st -X,r24
 2551               	/* #APP */
 2552               	 ;  328 "../SautCom.h" 1
 2553 0324 F894      		cli
 2554               	 ;  0 "" 2
 2555               	/* #NOAPP */
 2556 0326 8091 C900 		lds r24,201
 2557 032a 8062      		ori r24,lo8(32)
 2558 032c 8093 C900 		sts 201,r24
 2559               	.L123:
 2560 0330 1496      		adiw r26,4
 2561 0332 9C91      		ld r25,X
 2562 0334 1497      		sbiw r26,4
 2563 0336 00C0      		rjmp .L112
 2564               	.L116:
 2565 0338 9091 0000 		lds r25,data+28
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 46


 2566 033c 9695      		lsr r25
 2567 033e 9170      		andi r25,lo8(1)
 2568 0340 8091 0000 		lds r24,data+28
 2569 0344 8E7F      		andi r24,lo8(-2)
 2570 0346 892B      		or r24,r25
 2571 0348 8093 0000 		sts data+28,r24
 2572 034c F0E0      		ldi r31,lo8(0)
 2573 034e EF77      		andi r30,lo8(127)
 2574 0350 F070      		andi r31,hi8(127)
 2575 0352 8FB7      		in r24,__SREG__
 2576               	/* #APP */
 2577               	 ;  9 "D:\Programming\avr-cpp-libs\foreign/cpp/Chizhov/AVR/atomic.h" 1
 2578 0354 F894      		cli
 2579               	 ;  0 "" 2
 2580               	/* #NOAPP */
 2581 0356 EE0F      		lsl r30
 2582 0358 FF1F      		rol r31
 2583 035a E050      		subi r30,lo8(-(data))
 2584 035c F040      		sbci r31,hi8(-(data))
 2585 035e 2581      		ldd r18,Z+5
 2586 0360 3681      		ldd r19,Z+6
 2587 0362 8FBF      		out __SREG__,r24
 2588 0364 1696      		adiw r26,5+1
 2589 0366 3C93      		st X,r19
 2590 0368 2E93      		st -X,r18
 2591 036a 1597      		sbiw r26,5
 2592 036c 8091 C900 		lds r24,201
 2593 0370 8F7E      		andi r24,lo8(-17)
 2594 0372 8093 C900 		sts 201,r24
 2595 0376 5C98      		cbi 43-32,4
 2596 0378 1196      		adiw r26,1
 2597 037a 3C93      		st X,r19
 2598 037c 2E93      		st -X,r18
 2599               	/* #APP */
 2600               	 ;  358 "../SautCom.h" 1
 2601 037e F894      		cli
 2602               	 ;  0 "" 2
 2603               	/* #NOAPP */
 2604 0380 8091 C900 		lds r24,201
 2605 0384 8062      		ori r24,lo8(32)
 2606 0386 8093 C900 		sts 201,r24
 2607 038a 1496      		adiw r26,4
 2608 038c 9C91      		ld r25,X
 2609 038e 1497      		sbiw r26,4
 2610 0390 00C0      		rjmp .L112
 2611               	.L130:
 2612 0392 1596      		adiw r26,5
 2613 0394 8C91      		ld r24,X
 2614 0396 1597      		sbiw r26,5
 2615 0398 807F      		andi r24,lo8(-16)
 2616 039a 832B      		or r24,r19
 2617 039c 1596      		adiw r26,5
 2618 039e 8C93      		st X,r24
 2619 03a0 1597      		sbiw r26,5
 2620 03a2 00C0      		rjmp .L112
 2621               		.size	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
 2622               		.section	.text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 47


 2623               		.weak	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2624               		.type	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2625               	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcL
 2626               	/* prologue: function */
 2627               	/* frame size = 0 */
 2628 0000 0E94 0000 		call _ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Regi
 2629               	/* epilogue start */
 2630 0004 0895      		ret
 2631               		.size	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2632               		.section	.text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2633               		.weak	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2634               		.type	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2635               	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcL
 2636               	/* prologue: function */
 2637               	/* frame size = 0 */
 2638 0000 FC01      		movw r30,r24
 2639 0002 5C9A      		sbi 43-32,4
 2640 0004 8091 C900 		lds r24,201
 2641 0008 8061      		ori r24,lo8(16)
 2642 000a 8093 C900 		sts 201,r24
 2643 000e 8481      		ldd r24,Z+4
 2644 0010 8830      		cpi r24,lo8(8)
 2645 0012 01F0      		breq .L136
 2646 0014 0895      		ret
 2647               	.L136:
 2648 0016 8091 C800 		lds r24,200
 2649 001a 8160      		ori r24,lo8(1)
 2650 001c 8093 C800 		sts 200,r24
 2651 0020 5C9A      		sbi 43-32,4
 2652 0022 8091 C900 		lds r24,201
 2653 0026 8061      		ori r24,lo8(16)
 2654 0028 8093 C900 		sts 201,r24
 2655 002c 1482      		std Z+4,__zero_reg__
 2656 002e 0895      		ret
 2657               		.size	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2658               		.section	.text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2659               		.weak	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2660               		.type	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2661               	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcL
 2662               	/* prologue: function */
 2663               	/* frame size = 0 */
 2664 0000 DC01      		movw r26,r24
 2665 0002 1496      		adiw r26,4
 2666 0004 9C91      		ld r25,X
 2667 0006 1497      		sbiw r26,4
 2668 0008 9430      		cpi r25,lo8(4)
 2669 000a 01F0      		breq .L143
 2670 000c 9530      		cpi r25,lo8(5)
 2671 000e 01F0      		breq .L144
 2672 0010 9630      		cpi r25,lo8(6)
 2673 0012 01F0      		breq .L145
 2674 0014 9730      		cpi r25,lo8(7)
 2675 0016 01F4      		brne .L139
 2676 0018 ED91      		ld r30,X+
 2677 001a FC91      		ld r31,X
 2678 001c 1197      		sbiw r26,1
 2679 001e EF70      		andi r30,lo8(15)
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 48


 2680 0020 F070      		andi r31,hi8(15)
 2681 0022 E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2682 0024 F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2683 0026 8081      		ld r24,Z
 2684 0028 8093 CE00 		sts 206,r24
 2685 002c 8091 C900 		lds r24,201
 2686 0030 8F7D      		andi r24,lo8(-33)
 2687 0032 8093 C900 		sts 201,r24
 2688               	.L139:
 2689 0036 9F5F      		subi r25,lo8(-(1))
 2690 0038 1496      		adiw r26,4
 2691 003a 9C93      		st X,r25
 2692               	/* epilogue start */
 2693 003c 0895      		ret
 2694               	.L143:
 2695 003e ED91      		ld r30,X+
 2696 0040 FC91      		ld r31,X
 2697 0042 1197      		sbiw r26,1
 2698 0044 EF2F      		mov r30,r31
 2699 0046 FF27      		clr r31
 2700 0048 E295      		swap r30
 2701 004a EF70      		andi r30,0x0f
 2702 004c E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2703 004e F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2704 0050 8081      		ld r24,Z
 2705 0052 8093 CE00 		sts 206,r24
 2706 0056 9F5F      		subi r25,lo8(-(1))
 2707 0058 1496      		adiw r26,4
 2708 005a 9C93      		st X,r25
 2709 005c 0895      		ret
 2710               	.L144:
 2711 005e ED91      		ld r30,X+
 2712 0060 FC91      		ld r31,X
 2713 0062 1197      		sbiw r26,1
 2714 0064 EF2F      		mov r30,r31
 2715 0066 FF27      		clr r31
 2716 0068 EF70      		andi r30,lo8(15)
 2717 006a F070      		andi r31,hi8(15)
 2718 006c E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2719 006e F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2720 0070 8081      		ld r24,Z
 2721 0072 8093 CE00 		sts 206,r24
 2722 0076 9F5F      		subi r25,lo8(-(1))
 2723 0078 1496      		adiw r26,4
 2724 007a 9C93      		st X,r25
 2725 007c 0895      		ret
 2726               	.L145:
 2727 007e ED91      		ld r30,X+
 2728 0080 FC91      		ld r31,X
 2729 0082 1197      		sbiw r26,1
 2730 0084 F295      		swap r31
 2731 0086 E295      		swap r30
 2732 0088 EF70      		andi r30,0x0f
 2733 008a EF27      		eor r30,r31
 2734 008c FF70      		andi r31,0x0f
 2735 008e EF27      		eor r30,r31
 2736 0090 EF70      		andi r30,lo8(15)
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 49


 2737 0092 F070      		andi r31,hi8(15)
 2738 0094 E050      		subi r30,lo8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2739 0096 F040      		sbci r31,hi8(-(_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204E
 2740 0098 8081      		ld r24,Z
 2741 009a 8093 CE00 		sts 206,r24
 2742 009e 9F5F      		subi r25,lo8(-(1))
 2743 00a0 1496      		adiw r26,4
 2744 00a2 9C93      		st X,r25
 2745 00a4 0895      		ret
 2746               		.size	_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I
 2747               		.section	.text._ZN6srutil8DelegateIFvjEE14empty_functionEPvj,"ax",@progbits
 2748               		.weak	_ZN6srutil8DelegateIFvjEE14empty_functionEPvj
 2749               		.type	_ZN6srutil8DelegateIFvjEE14empty_functionEPvj, @function
 2750               	_ZN6srutil8DelegateIFvjEE14empty_functionEPvj:
 2751               	/* prologue: function */
 2752               	/* frame size = 0 */
 2753               	/* epilogue start */
 2754 0000 0895      		ret
 2755               		.size	_ZN6srutil8DelegateIFvjEE14empty_functionEPvj, .-_ZN6srutil8DelegateIFvjEE14empty_functionEP
 2756               		.section	.text._GLOBAL__I_INT0_handler,"ax",@progbits
 2757               		.type	_GLOBAL__I_INT0_handler, @function
 2758               	_GLOBAL__I_INT0_handler:
 2759               	/* prologue: function */
 2760               	/* frame size = 0 */
 2761 0000 1092 0000 		sts (INT0_handler)+1,__zero_reg__
 2762 0004 1092 0000 		sts INT0_handler,__zero_reg__
 2763 0008 20E0      		ldi r18,lo8(gs(_ZN6srutil8DelegateIFvvEE14empty_functionEPv))
 2764 000a 30E0      		ldi r19,hi8(gs(_ZN6srutil8DelegateIFvvEE14empty_functionEPv))
 2765 000c 3093 0000 		sts (INT0_handler+2)+1,r19
 2766 0010 2093 0000 		sts INT0_handler+2,r18
 2767 0014 1092 0000 		sts (INT1_handler)+1,__zero_reg__
 2768 0018 1092 0000 		sts INT1_handler,__zero_reg__
 2769 001c 3093 0000 		sts (INT1_handler+2)+1,r19
 2770 0020 2093 0000 		sts INT1_handler+2,r18
 2771 0024 1092 0000 		sts (INT2_handler)+1,__zero_reg__
 2772 0028 1092 0000 		sts INT2_handler,__zero_reg__
 2773 002c 3093 0000 		sts (INT2_handler+2)+1,r19
 2774 0030 2093 0000 		sts INT2_handler+2,r18
 2775 0034 1092 0000 		sts (INT3_handler)+1,__zero_reg__
 2776 0038 1092 0000 		sts INT3_handler,__zero_reg__
 2777 003c 3093 0000 		sts (INT3_handler+2)+1,r19
 2778 0040 2093 0000 		sts INT3_handler+2,r18
 2779 0044 1092 0000 		sts (INT4_handler)+1,__zero_reg__
 2780 0048 1092 0000 		sts INT4_handler,__zero_reg__
 2781 004c 3093 0000 		sts (INT4_handler+2)+1,r19
 2782 0050 2093 0000 		sts INT4_handler+2,r18
 2783 0054 1092 0000 		sts (INT5_handler)+1,__zero_reg__
 2784 0058 1092 0000 		sts INT5_handler,__zero_reg__
 2785 005c 3093 0000 		sts (INT5_handler+2)+1,r19
 2786 0060 2093 0000 		sts INT5_handler+2,r18
 2787 0064 1092 0000 		sts (INT6_handler)+1,__zero_reg__
 2788 0068 1092 0000 		sts INT6_handler,__zero_reg__
 2789 006c 3093 0000 		sts (INT6_handler+2)+1,r19
 2790 0070 2093 0000 		sts INT6_handler+2,r18
 2791 0074 1092 0000 		sts (INT7_handler)+1,__zero_reg__
 2792 0078 1092 0000 		sts INT7_handler,__zero_reg__
 2793 007c 3093 0000 		sts (INT7_handler+2)+1,r19
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 50


 2794 0080 2093 0000 		sts INT7_handler+2,r18
 2795 0084 1092 0000 		sts (TIMER2_COMP_handler)+1,__zero_reg__
 2796 0088 1092 0000 		sts TIMER2_COMP_handler,__zero_reg__
 2797 008c 3093 0000 		sts (TIMER2_COMP_handler+2)+1,r19
 2798 0090 2093 0000 		sts TIMER2_COMP_handler+2,r18
 2799 0094 1092 0000 		sts (TIMER2_OVF_handler)+1,__zero_reg__
 2800 0098 1092 0000 		sts TIMER2_OVF_handler,__zero_reg__
 2801 009c 3093 0000 		sts (TIMER2_OVF_handler+2)+1,r19
 2802 00a0 2093 0000 		sts TIMER2_OVF_handler+2,r18
 2803 00a4 1092 0000 		sts (TIMER1_CAPT_handler)+1,__zero_reg__
 2804 00a8 1092 0000 		sts TIMER1_CAPT_handler,__zero_reg__
 2805 00ac 3093 0000 		sts (TIMER1_CAPT_handler+2)+1,r19
 2806 00b0 2093 0000 		sts TIMER1_CAPT_handler+2,r18
 2807 00b4 1092 0000 		sts (TIMER1_COMPA_handler)+1,__zero_reg__
 2808 00b8 1092 0000 		sts TIMER1_COMPA_handler,__zero_reg__
 2809 00bc 3093 0000 		sts (TIMER1_COMPA_handler+2)+1,r19
 2810 00c0 2093 0000 		sts TIMER1_COMPA_handler+2,r18
 2811 00c4 1092 0000 		sts (TIMER1_COMPB_handler)+1,__zero_reg__
 2812 00c8 1092 0000 		sts TIMER1_COMPB_handler,__zero_reg__
 2813 00cc 3093 0000 		sts (TIMER1_COMPB_handler+2)+1,r19
 2814 00d0 2093 0000 		sts TIMER1_COMPB_handler+2,r18
 2815 00d4 1092 0000 		sts (TIMER1_COMPC_handler)+1,__zero_reg__
 2816 00d8 1092 0000 		sts TIMER1_COMPC_handler,__zero_reg__
 2817 00dc 3093 0000 		sts (TIMER1_COMPC_handler+2)+1,r19
 2818 00e0 2093 0000 		sts TIMER1_COMPC_handler+2,r18
 2819 00e4 1092 0000 		sts (TIMER1_OVF_handler)+1,__zero_reg__
 2820 00e8 1092 0000 		sts TIMER1_OVF_handler,__zero_reg__
 2821 00ec 3093 0000 		sts (TIMER1_OVF_handler+2)+1,r19
 2822 00f0 2093 0000 		sts TIMER1_OVF_handler+2,r18
 2823 00f4 1092 0000 		sts (TIMER0_COMP_handler)+1,__zero_reg__
 2824 00f8 1092 0000 		sts TIMER0_COMP_handler,__zero_reg__
 2825 00fc 3093 0000 		sts (TIMER0_COMP_handler+2)+1,r19
 2826 0100 2093 0000 		sts TIMER0_COMP_handler+2,r18
 2827 0104 1092 0000 		sts (TIMER0_OVF_handler)+1,__zero_reg__
 2828 0108 1092 0000 		sts TIMER0_OVF_handler,__zero_reg__
 2829 010c 3093 0000 		sts (TIMER0_OVF_handler+2)+1,r19
 2830 0110 2093 0000 		sts TIMER0_OVF_handler+2,r18
 2831 0114 1092 0000 		sts (CANIT_handler)+1,__zero_reg__
 2832 0118 1092 0000 		sts CANIT_handler,__zero_reg__
 2833 011c 3093 0000 		sts (CANIT_handler+2)+1,r19
 2834 0120 2093 0000 		sts CANIT_handler+2,r18
 2835 0124 1092 0000 		sts (OVRIT_handler)+1,__zero_reg__
 2836 0128 1092 0000 		sts OVRIT_handler,__zero_reg__
 2837 012c 3093 0000 		sts (OVRIT_handler+2)+1,r19
 2838 0130 2093 0000 		sts OVRIT_handler+2,r18
 2839 0134 1092 0000 		sts (SPI_STC_handler)+1,__zero_reg__
 2840 0138 1092 0000 		sts SPI_STC_handler,__zero_reg__
 2841 013c 3093 0000 		sts (SPI_STC_handler+2)+1,r19
 2842 0140 2093 0000 		sts SPI_STC_handler+2,r18
 2843 0144 1092 0000 		sts (USART0_RX_handler)+1,__zero_reg__
 2844 0148 1092 0000 		sts USART0_RX_handler,__zero_reg__
 2845 014c 3093 0000 		sts (USART0_RX_handler+2)+1,r19
 2846 0150 2093 0000 		sts USART0_RX_handler+2,r18
 2847 0154 1092 0000 		sts (USART0_UDRE_handler)+1,__zero_reg__
 2848 0158 1092 0000 		sts USART0_UDRE_handler,__zero_reg__
 2849 015c 3093 0000 		sts (USART0_UDRE_handler+2)+1,r19
 2850 0160 2093 0000 		sts USART0_UDRE_handler+2,r18
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 51


 2851 0164 1092 0000 		sts (USART0_TX_handler)+1,__zero_reg__
 2852 0168 1092 0000 		sts USART0_TX_handler,__zero_reg__
 2853 016c 3093 0000 		sts (USART0_TX_handler+2)+1,r19
 2854 0170 2093 0000 		sts USART0_TX_handler+2,r18
 2855 0174 1092 0000 		sts (ANALOG_COMP_handler)+1,__zero_reg__
 2856 0178 1092 0000 		sts ANALOG_COMP_handler,__zero_reg__
 2857 017c 3093 0000 		sts (ANALOG_COMP_handler+2)+1,r19
 2858 0180 2093 0000 		sts ANALOG_COMP_handler+2,r18
 2859 0184 1092 0000 		sts (ADC_handler)+1,__zero_reg__
 2860 0188 1092 0000 		sts ADC_handler,__zero_reg__
 2861 018c 3093 0000 		sts (ADC_handler+2)+1,r19
 2862 0190 2093 0000 		sts ADC_handler+2,r18
 2863 0194 1092 0000 		sts (EE_READY_handler)+1,__zero_reg__
 2864 0198 1092 0000 		sts EE_READY_handler,__zero_reg__
 2865 019c 3093 0000 		sts (EE_READY_handler+2)+1,r19
 2866 01a0 2093 0000 		sts EE_READY_handler+2,r18
 2867 01a4 1092 0000 		sts (TIMER3_CAPT_handler)+1,__zero_reg__
 2868 01a8 1092 0000 		sts TIMER3_CAPT_handler,__zero_reg__
 2869 01ac 3093 0000 		sts (TIMER3_CAPT_handler+2)+1,r19
 2870 01b0 2093 0000 		sts TIMER3_CAPT_handler+2,r18
 2871 01b4 1092 0000 		sts (TIMER3_COMPA_handler)+1,__zero_reg__
 2872 01b8 1092 0000 		sts TIMER3_COMPA_handler,__zero_reg__
 2873 01bc 3093 0000 		sts (TIMER3_COMPA_handler+2)+1,r19
 2874 01c0 2093 0000 		sts TIMER3_COMPA_handler+2,r18
 2875 01c4 1092 0000 		sts (TIMER3_COMPB_handler)+1,__zero_reg__
 2876 01c8 1092 0000 		sts TIMER3_COMPB_handler,__zero_reg__
 2877 01cc 3093 0000 		sts (TIMER3_COMPB_handler+2)+1,r19
 2878 01d0 2093 0000 		sts TIMER3_COMPB_handler+2,r18
 2879 01d4 1092 0000 		sts (TIMER3_COMPC_handler)+1,__zero_reg__
 2880 01d8 1092 0000 		sts TIMER3_COMPC_handler,__zero_reg__
 2881 01dc 3093 0000 		sts (TIMER3_COMPC_handler+2)+1,r19
 2882 01e0 2093 0000 		sts TIMER3_COMPC_handler+2,r18
 2883 01e4 1092 0000 		sts (TIMER3_OVF_handler)+1,__zero_reg__
 2884 01e8 1092 0000 		sts TIMER3_OVF_handler,__zero_reg__
 2885 01ec 3093 0000 		sts (TIMER3_OVF_handler+2)+1,r19
 2886 01f0 2093 0000 		sts TIMER3_OVF_handler+2,r18
 2887 01f4 1092 0000 		sts (TWI_handler)+1,__zero_reg__
 2888 01f8 1092 0000 		sts TWI_handler,__zero_reg__
 2889 01fc 3093 0000 		sts (TWI_handler+2)+1,r19
 2890 0200 2093 0000 		sts TWI_handler+2,r18
 2891 0204 1092 0000 		sts (SPM_READY_handler)+1,__zero_reg__
 2892 0208 1092 0000 		sts SPM_READY_handler,__zero_reg__
 2893 020c 3093 0000 		sts (SPM_READY_handler+2)+1,r19
 2894 0210 2093 0000 		sts SPM_READY_handler+2,r18
 2895 0214 E0E0      		ldi r30,lo8(dispatcher)
 2896 0216 F0E0      		ldi r31,hi8(dispatcher)
 2897 0218 80E0      		ldi r24,lo8(gs(_ZN6srutil8DelegateIFvjEE14empty_functionEPvj))
 2898 021a 90E0      		ldi r25,hi8(gs(_ZN6srutil8DelegateIFvjEE14empty_functionEPvj))
 2899               	.L149:
 2900 021c 1182      		std Z+1,__zero_reg__
 2901 021e 1082      		st Z,__zero_reg__
 2902 0220 9383      		std Z+3,r25
 2903 0222 8283      		std Z+2,r24
 2904 0224 3696      		adiw r30,6
 2905 0226 40E0      		ldi r20,hi8(dispatcher+1536)
 2906 0228 E030      		cpi r30,lo8(dispatcher+1536)
 2907 022a F407      		cpc r31,r20
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 52


 2908 022c 01F4      		brne .L149
 2909 022e 1092 0000 		sts (data)+1,__zero_reg__
 2910 0232 1092 0000 		sts data,__zero_reg__
 2911 0236 3093 0000 		sts (data+2)+1,r19
 2912 023a 2093 0000 		sts data+2,r18
 2913 023e 1092 0000 		sts (data+5)+1,__zero_reg__
 2914 0242 1092 0000 		sts data+5,__zero_reg__
 2915 0246 1092 0000 		sts (data+7)+1,__zero_reg__
 2916 024a 1092 0000 		sts data+7,__zero_reg__
 2917 024e 1092 0000 		sts (data+9)+1,__zero_reg__
 2918 0252 1092 0000 		sts data+9,__zero_reg__
 2919 0256 1092 0000 		sts (data+11)+1,__zero_reg__
 2920 025a 1092 0000 		sts data+11,__zero_reg__
 2921 025e 1092 0000 		sts (data+13)+1,__zero_reg__
 2922 0262 1092 0000 		sts data+13,__zero_reg__
 2923 0266 1092 0000 		sts (data+15)+1,__zero_reg__
 2924 026a 1092 0000 		sts data+15,__zero_reg__
 2925 026e 1092 0000 		sts (data+17)+1,__zero_reg__
 2926 0272 1092 0000 		sts data+17,__zero_reg__
 2927 0276 1092 0000 		sts (data+19)+1,__zero_reg__
 2928 027a 1092 0000 		sts data+19,__zero_reg__
 2929 027e 1092 0000 		sts (data+21)+1,__zero_reg__
 2930 0282 1092 0000 		sts data+21,__zero_reg__
 2931 0286 1092 0000 		sts (data+24)+1,__zero_reg__
 2932 028a 1092 0000 		sts data+24,__zero_reg__
 2933 028e 3093 0000 		sts (data+26)+1,r19
 2934 0292 2093 0000 		sts data+26,r18
 2935 0296 1092 0000 		sts (com+5)+1,__zero_reg__
 2936 029a 1092 0000 		sts com+5,__zero_reg__
 2937 029e 1092 0000 		sts com+7,__zero_reg__
 2938 02a2 539A      		sbi 42-32,3
 2939 02a4 5B9A      		sbi 43-32,3
 2940 02a6 5298      		cbi 42-32,2
 2941 02a8 5A9A      		sbi 43-32,2
 2942 02aa 549A      		sbi 42-32,4
 2943 02ac 2798      		cbi 36-32,7
 2944 02ae 2F9A      		sbi 37-32,7
 2945 02b0 80E0      		ldi r24,lo8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2946 02b2 90E0      		ldi r25,hi8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2947 02b4 9093 0000 		sts (USART1_RX_handler+2)+1,r25
 2948 02b8 8093 0000 		sts USART1_RX_handler+2,r24
 2949 02bc 20E0      		ldi r18,lo8(com)
 2950 02be 30E0      		ldi r19,hi8(com)
 2951 02c0 3093 0000 		sts (USART1_RX_handler)+1,r19
 2952 02c4 2093 0000 		sts USART1_RX_handler,r18
 2953 02c8 80E0      		ldi r24,lo8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2954 02ca 90E0      		ldi r25,hi8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2955 02cc 9093 0000 		sts (USART1_TX_handler+2)+1,r25
 2956 02d0 8093 0000 		sts USART1_TX_handler+2,r24
 2957 02d4 3093 0000 		sts (USART1_TX_handler)+1,r19
 2958 02d8 2093 0000 		sts USART1_TX_handler,r18
 2959 02dc 80E0      		ldi r24,lo8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2960 02de 90E0      		ldi r25,hi8(gs(_ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj
 2961 02e0 9093 0000 		sts (USART1_UDRE_handler+2)+1,r25
 2962 02e4 8093 0000 		sts USART1_UDRE_handler+2,r24
 2963 02e8 3093 0000 		sts (USART1_UDRE_handler)+1,r19
 2964 02ec 2093 0000 		sts USART1_UDRE_handler,r18
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 53


 2965 02f0 8091 C800 		lds r24,200
 2966 02f4 8260      		ori r24,lo8(2)
 2967 02f6 8093 C800 		sts 200,r24
 2968 02fa 8CE0      		ldi r24,lo8(12)
 2969 02fc 8093 CC00 		sts 204,r24
 2970 0300 8091 CD00 		lds r24,205
 2971 0304 807F      		andi r24,lo8(-16)
 2972 0306 ECEC      		ldi r30,lo8(204)
 2973 0308 F0E0      		ldi r31,hi8(204)
 2974 030a 8183      		std Z+1,r24
 2975 030c 9091 C900 		lds r25,201
 2976 0310 8091 CA00 		lds r24,202
 2977 0314 8660      		ori r24,lo8(6)
 2978 0316 8778      		andi r24,lo8(-121)
 2979 0318 8093 CA00 		sts 202,r24
 2980 031c 9C6C      		ori r25,lo8(-52)
 2981 031e 9093 C900 		sts 201,r25
 2982 0322 81E0      		ldi r24,lo8(1)
 2983 0324 8093 0000 		sts com+2,r24
 2984 0328 8091 C800 		lds r24,200
 2985 032c 8160      		ori r24,lo8(1)
 2986 032e 8093 C800 		sts 200,r24
 2987 0332 5C9A      		sbi 43-32,4
 2988 0334 8091 C900 		lds r24,201
 2989 0338 8061      		ori r24,lo8(16)
 2990 033a 8093 C900 		sts 201,r24
 2991 033e 1092 0000 		sts com+4,__zero_reg__
 2992               	/* epilogue start */
 2993 0342 0895      		ret
 2994               		.size	_GLOBAL__I_INT0_handler, .-_GLOBAL__I_INT0_handler
 2995               		.global __do_global_ctors
 2996               		.section .ctors,"a",@progbits
 2997 0000 0000      		.word	gs(_GLOBAL__I_INT0_handler)
 2998               		.section	.text._Z13commandParserv,"ax",@progbits
 2999               	.global	_Z13commandParserv
 3000               		.type	_Z13commandParserv, @function
 3001               	_Z13commandParserv:
 3002 0000 DF92      		push r13
 3003 0002 EF92      		push r14
 3004 0004 FF92      		push r15
 3005 0006 0F93      		push r16
 3006 0008 1F93      		push r17
 3007 000a DF93      		push r29
 3008 000c CF93      		push r28
 3009 000e CDB7      		in r28,__SP_L__
 3010 0010 DEB7      		in r29,__SP_H__
 3011 0012 6397      		sbiw r28,19
 3012 0014 0FB6      		in __tmp_reg__,__SREG__
 3013 0016 F894      		cli
 3014 0018 DEBF      		out __SP_H__,r29
 3015 001a 0FBE      		out __SREG__,__tmp_reg__
 3016 001c CDBF      		out __SP_L__,r28
 3017               	/* prologue: function */
 3018               	/* frame size = 19 */
 3019 001e 8CE0      		ldi r24,lo8(12)
 3020 0020 8A83      		std Y+2,r24
 3021 0022 60E0      		ldi r22,lo8(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEE
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 54


 3022 0024 70E0      		ldi r23,hi8(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEE
 3023 0026 FB01      		movw r30,r22
 3024               	/* #APP */
 3025               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3026 0028 8491      		lpm r24, Z
 3027               		
 3028               	 ;  0 "" 2
 3029               	/* #NOAPP */
 3030 002a E82F      		mov r30,r24
 3031 002c F0E0      		ldi r31,lo8(0)
 3032 002e EF77      		andi r30,lo8(127)
 3033 0030 F070      		andi r31,hi8(127)
 3034 0032 EE0F      		lsl r30
 3035 0034 FF1F      		rol r31
 3036 0036 E050      		subi r30,lo8(-(data+5))
 3037 0038 F040      		sbci r31,hi8(-(data+5))
 3038 003a 8181      		ldd r24,Z+1
 3039 003c 8983      		std Y+1,r24
 3040 003e 87FF      		sbrs r24,7
 3041 0040 00C0      		rjmp .L153
 3042 0042 81E0      		ldi r24,lo8(1)
 3043 0044 8093 0000 		sts com+2,r24
 3044               	.L153:
 3045 0048 5981      		ldd r21,Y+1
 3046 004a 56FF      		sbrs r21,6
 3047 004c 00C0      		rjmp .L158
 3048 004e A0E0      		ldi r26,lo8(ID+5)
 3049 0050 B0E0      		ldi r27,hi8(ID+5)
 3050 0052 FD01      		movw r30,r26
 3051               	/* #APP */
 3052               	 ;  198 "../main.cpp" 1
 3053 0054 4491      		lpm r20, Z
 3054               		
 3055               	 ;  0 "" 2
 3056               	/* #NOAPP */
 3057 0056 1196      		adiw r26,1
 3058 0058 FD01      		movw r30,r26
 3059               	/* #APP */
 3060               	 ;  198 "../main.cpp" 1
 3061 005a 2491      		lpm r18, Z
 3062               		
 3063               	 ;  0 "" 2
 3064               	/* #NOAPP */
 3065 005c 942F      		mov r25,r20
 3066 005e 80E0      		ldi r24,lo8(0)
 3067 0060 30E0      		ldi r19,lo8(0)
 3068 0062 282B      		or r18,r24
 3069 0064 392B      		or r19,r25
 3070 0066 3B87      		std Y+11,r19
 3071 0068 2A87      		std Y+10,r18
 3072 006a 3093 0000 		sts (data+21)+1,r19
 3073 006e 2093 0000 		sts data+21,r18
 3074 0072 1196      		adiw r26,1
 3075 0074 FD01      		movw r30,r26
 3076               	/* #APP */
 3077               	 ;  199 "../main.cpp" 1
 3078 0076 4491      		lpm r20, Z
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 55


 3079               		
 3080               	 ;  0 "" 2
 3081               	/* #NOAPP */
 3082 0078 1196      		adiw r26,1
 3083 007a FD01      		movw r30,r26
 3084               	/* #APP */
 3085               	 ;  199 "../main.cpp" 1
 3086 007c 2491      		lpm r18, Z
 3087               		
 3088               	 ;  0 "" 2
 3089               	/* #NOAPP */
 3090 007e 942F      		mov r25,r20
 3091 0080 80E0      		ldi r24,lo8(0)
 3092 0082 30E0      		ldi r19,lo8(0)
 3093 0084 282B      		or r18,r24
 3094 0086 392B      		or r19,r25
 3095 0088 3D87      		std Y+13,r19
 3096 008a 2C87      		std Y+12,r18
 3097 008c 3093 0000 		sts (data+19)+1,r19
 3098 0090 2093 0000 		sts data+19,r18
 3099 0094 1196      		adiw r26,1
 3100 0096 FD01      		movw r30,r26
 3101               	/* #APP */
 3102               	 ;  200 "../main.cpp" 1
 3103 0098 2591      		lpm r18, Z+
 3104 009a 3491      		lpm r19, Z
 3105               		
 3106               	 ;  0 "" 2
 3107               	/* #NOAPP */
 3108 009c C901      		movw r24,r18
 3109 009e 8F70      		andi r24,lo8(15)
 3110 00a0 9070      		andi r25,hi8(15)
 3111 00a2 982F      		mov r25,r24
 3112 00a4 8827      		clr r24
 3113 00a6 232F      		mov r18,r19
 3114 00a8 3327      		clr r19
 3115 00aa 822B      		or r24,r18
 3116 00ac 932B      		or r25,r19
 3117 00ae 9F87      		std Y+15,r25
 3118 00b0 8E87      		std Y+14,r24
 3119 00b2 9093 0000 		sts (data+17)+1,r25
 3120 00b6 8093 0000 		sts data+17,r24
 3121 00ba 1296      		adiw r26,2
 3122 00bc FD01      		movw r30,r26
 3123               	/* #APP */
 3124               	 ;  201 "../main.cpp" 1
 3125 00be 2591      		lpm r18, Z+
 3126 00c0 3491      		lpm r19, Z
 3127               		
 3128               	 ;  0 "" 2
 3129               	/* #NOAPP */
 3130 00c2 C901      		movw r24,r18
 3131 00c4 8F70      		andi r24,lo8(15)
 3132 00c6 9070      		andi r25,hi8(15)
 3133 00c8 982F      		mov r25,r24
 3134 00ca 8827      		clr r24
 3135 00cc 232F      		mov r18,r19
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 56


 3136 00ce 3327      		clr r19
 3137 00d0 822B      		or r24,r18
 3138 00d2 932B      		or r25,r19
 3139 00d4 998B      		std Y+17,r25
 3140 00d6 888B      		std Y+16,r24
 3141 00d8 9093 0000 		sts (data+15)+1,r25
 3142 00dc 8093 0000 		sts data+15,r24
 3143 00e0 55FD      		sbrc r21,5
 3144 00e2 00C0      		rjmp .L155
 3145               	.L156:
 3146               	/* epilogue start */
 3147 00e4 6396      		adiw r28,19
 3148 00e6 0FB6      		in __tmp_reg__,__SREG__
 3149 00e8 F894      		cli
 3150 00ea DEBF      		out __SP_H__,r29
 3151 00ec 0FBE      		out __SREG__,__tmp_reg__
 3152 00ee CDBF      		out __SP_L__,r28
 3153 00f0 CF91      		pop r28
 3154 00f2 DF91      		pop r29
 3155 00f4 1F91      		pop r17
 3156 00f6 0F91      		pop r16
 3157 00f8 FF90      		pop r15
 3158 00fa EF90      		pop r14
 3159 00fc DF90      		pop r13
 3160 00fe 0895      		ret
 3161               	.L158:
 3162 0100 55FF      		sbrs r21,5
 3163 0102 00C0      		rjmp .L159
 3164               	.L155:
 3165 0104 8CE0      		ldi r24,lo8(12)
 3166 0106 8B83      		std Y+3,r24
 3167 0108 FB01      		movw r30,r22
 3168               	/* #APP */
 3169               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3170 010a 8491      		lpm r24, Z
 3171               		
 3172               	 ;  0 "" 2
 3173               	/* #NOAPP */
 3174 010c E82F      		mov r30,r24
 3175 010e F0E0      		ldi r31,lo8(0)
 3176 0110 EF77      		andi r30,lo8(127)
 3177 0112 F070      		andi r31,hi8(127)
 3178 0114 EE0F      		lsl r30
 3179 0116 FF1F      		rol r31
 3180 0118 E050      		subi r30,lo8(-(data))
 3181 011a F040      		sbci r31,hi8(-(data))
 3182 011c 0581      		ldd r16,Z+5
 3183 011e 1681      		ldd r17,Z+6
 3184 0120 1070      		andi r17,hi8(255)
 3185 0122 000F      		lsl r16
 3186 0124 111F      		rol r17
 3187 0126 000F      		lsl r16
 3188 0128 111F      		rol r17
 3189 012a 000F      		lsl r16
 3190 012c 111F      		rol r17
 3191 012e 88E0      		ldi r24,lo8(8)
 3192 0130 8C83      		std Y+4,r24
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 57


 3193 0132 B0E0      		ldi r27,lo8(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEE
 3194 0134 EB2E      		mov r14,r27
 3195 0136 B0E0      		ldi r27,hi8(_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEE
 3196 0138 FB2E      		mov r15,r27
 3197 013a F701      		movw r30,r14
 3198               	/* #APP */
 3199               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3200 013c D490      		lpm r13, Z
 3201               		
 3202               	 ;  0 "" 2
 3203               	/* #NOAPP */
 3204 013e C801      		movw r24,r16
 3205 0140 0E94 0000 		call __eerd_word_can128
 3206 0144 9B8B      		std Y+19,r25
 3207 0146 8A8B      		std Y+18,r24
 3208 0148 ED2D      		mov r30,r13
 3209 014a F0E0      		ldi r31,lo8(0)
 3210 014c EF77      		andi r30,lo8(127)
 3211 014e F070      		andi r31,hi8(127)
 3212 0150 EE0F      		lsl r30
 3213 0152 FF1F      		rol r31
 3214 0154 E050      		subi r30,lo8(-(data+5))
 3215 0156 F040      		sbci r31,hi8(-(data+5))
 3216 0158 9183      		std Z+1,r25
 3217 015a 8083      		st Z,r24
 3218 015c 0C5F      		subi r16,lo8(-(4))
 3219 015e 1F4F      		sbci r17,hi8(-(4))
 3220 0160 89E0      		ldi r24,lo8(9)
 3221 0162 8C83      		std Y+4,r24
 3222 0164 0894      		sec
 3223 0166 E11C      		adc r14,__zero_reg__
 3224 0168 F11C      		adc r15,__zero_reg__
 3225 016a F701      		movw r30,r14
 3226               	/* #APP */
 3227               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3228 016c D490      		lpm r13, Z
 3229               		
 3230               	 ;  0 "" 2
 3231               	/* #NOAPP */
 3232 016e C801      		movw r24,r16
 3233 0170 0E94 0000 		call __eerd_word_can128
 3234 0174 9B8B      		std Y+19,r25
 3235 0176 8A8B      		std Y+18,r24
 3236 0178 ED2D      		mov r30,r13
 3237 017a F0E0      		ldi r31,lo8(0)
 3238 017c EF77      		andi r30,lo8(127)
 3239 017e F070      		andi r31,hi8(127)
 3240 0180 EE0F      		lsl r30
 3241 0182 FF1F      		rol r31
 3242 0184 E050      		subi r30,lo8(-(data+5))
 3243 0186 F040      		sbci r31,hi8(-(data+5))
 3244 0188 9183      		std Z+1,r25
 3245 018a 8083      		st Z,r24
 3246 018c 0C5F      		subi r16,lo8(-(4))
 3247 018e 1F4F      		sbci r17,hi8(-(4))
 3248 0190 8AE0      		ldi r24,lo8(10)
 3249 0192 8C83      		std Y+4,r24
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 58


 3250 0194 0894      		sec
 3251 0196 E11C      		adc r14,__zero_reg__
 3252 0198 F11C      		adc r15,__zero_reg__
 3253 019a F701      		movw r30,r14
 3254               	/* #APP */
 3255               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3256 019c D490      		lpm r13, Z
 3257               		
 3258               	 ;  0 "" 2
 3259               	/* #NOAPP */
 3260 019e C801      		movw r24,r16
 3261 01a0 0E94 0000 		call __eerd_word_can128
 3262 01a4 9B8B      		std Y+19,r25
 3263 01a6 8A8B      		std Y+18,r24
 3264 01a8 ED2D      		mov r30,r13
 3265 01aa F0E0      		ldi r31,lo8(0)
 3266 01ac EF77      		andi r30,lo8(127)
 3267 01ae F070      		andi r31,hi8(127)
 3268 01b0 EE0F      		lsl r30
 3269 01b2 FF1F      		rol r31
 3270 01b4 E050      		subi r30,lo8(-(data+5))
 3271 01b6 F040      		sbci r31,hi8(-(data+5))
 3272 01b8 9183      		std Z+1,r25
 3273 01ba 8083      		st Z,r24
 3274 01bc 8BE0      		ldi r24,lo8(11)
 3275 01be 8C83      		std Y+4,r24
 3276 01c0 0894      		sec
 3277 01c2 E11C      		adc r14,__zero_reg__
 3278 01c4 F11C      		adc r15,__zero_reg__
 3279 01c6 F701      		movw r30,r14
 3280               	/* #APP */
 3281               	 ;  277 "D:\Programming\avr-cpp-libs\my/cpp/map-static.h" 1
 3282 01c8 F490      		lpm r15, Z
 3283               		
 3284               	 ;  0 "" 2
 3285               	/* #NOAPP */
 3286 01ca C801      		movw r24,r16
 3287 01cc 0496      		adiw r24,4
 3288 01ce 0E94 0000 		call __eerd_word_can128
 3289 01d2 EF2D      		mov r30,r15
 3290 01d4 F0E0      		ldi r31,lo8(0)
 3291 01d6 EF77      		andi r30,lo8(127)
 3292 01d8 F070      		andi r31,hi8(127)
 3293 01da EE0F      		lsl r30
 3294 01dc FF1F      		rol r31
 3295 01de E050      		subi r30,lo8(-(data+5))
 3296 01e0 F040      		sbci r31,hi8(-(data+5))
 3297 01e2 9183      		std Z+1,r25
 3298 01e4 8083      		st Z,r24
 3299 01e6 00C0      		rjmp .L156
 3300               	.L159:
 3301 01e8 1092 0000 		sts (data+21)+1,__zero_reg__
 3302 01ec 1092 0000 		sts data+21,__zero_reg__
 3303 01f0 1092 0000 		sts (data+19)+1,__zero_reg__
 3304 01f4 1092 0000 		sts data+19,__zero_reg__
 3305 01f8 00C0      		rjmp .L156
 3306               		.size	_Z13commandParserv, .-_Z13commandParserv
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 59


 3307               		.section	.text._Z8bandDiamPKhS0_,"ax",@progbits
 3308               	.global	_Z8bandDiamPKhS0_
 3309               		.type	_Z8bandDiamPKhS0_, @function
 3310               	_Z8bandDiamPKhS0_:
 3311 0000 FF92      		push r15
 3312 0002 0F93      		push r16
 3313 0004 1F93      		push r17
 3314               	/* prologue: function */
 3315               	/* frame size = 0 */
 3316 0006 8B01      		movw r16,r22
 3317 0008 0E94 0000 		call __eerd_byte_can128
 3318 000c F82E      		mov r15,r24
 3319 000e C801      		movw r24,r16
 3320 0010 0E94 0000 		call __eerd_byte_can128
 3321 0014 482F      		mov r20,r24
 3322 0016 87FD      		sbrc r24,7
 3323 0018 00C0      		rjmp .L164
 3324 001a 4F77      		andi r20,lo8(127)
 3325 001c 8F2D      		mov r24,r15
 3326 001e 90E0      		ldi r25,lo8(0)
 3327 0020 9C01      		movw r18,r24
 3328 0022 220F      		lsl r18
 3329 0024 331F      		rol r19
 3330 0026 220F      		lsl r18
 3331 0028 331F      		rol r19
 3332 002a 220F      		lsl r18
 3333 002c 331F      		rol r19
 3334 002e 880F      		lsl r24
 3335 0030 991F      		rol r25
 3336 0032 280F      		add r18,r24
 3337 0034 391F      		adc r19,r25
 3338 0036 240F      		add r18,r20
 3339 0038 311D      		adc r19,__zero_reg__
 3340 003a C901      		movw r24,r18
 3341               	/* epilogue start */
 3342 003c 1F91      		pop r17
 3343 003e 0F91      		pop r16
 3344 0040 FF90      		pop r15
 3345 0042 0895      		ret
 3346               	.L164:
 3347 0044 8F2D      		mov r24,r15
 3348 0046 90E0      		ldi r25,lo8(0)
 3349 0048 9C01      		movw r18,r24
 3350 004a 220F      		lsl r18
 3351 004c 331F      		rol r19
 3352 004e 220F      		lsl r18
 3353 0050 331F      		rol r19
 3354 0052 220F      		lsl r18
 3355 0054 331F      		rol r19
 3356 0056 880F      		lsl r24
 3357 0058 991F      		rol r25
 3358 005a 280F      		add r18,r24
 3359 005c 391F      		adc r19,r25
 3360 005e 4F77      		andi r20,lo8(127)
 3361 0060 241B      		sub r18,r20
 3362 0062 3109      		sbc r19,__zero_reg__
 3363 0064 C901      		movw r24,r18
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 60


 3364 0066 1F91      		pop r17
 3365 0068 0F91      		pop r16
 3366 006a FF90      		pop r15
 3367 006c 0895      		ret
 3368               		.size	_Z8bandDiamPKhS0_, .-_Z8bandDiamPKhS0_
 3369               		.section	.text._ZdaPv,"ax",@progbits
 3370               	.global	_ZdaPv
 3371               		.type	_ZdaPv, @function
 3372               	_ZdaPv:
 3373               	/* prologue: function */
 3374               	/* frame size = 0 */
 3375 0000 0E94 0000 		call free
 3376               	/* epilogue start */
 3377 0004 0895      		ret
 3378               		.size	_ZdaPv, .-_ZdaPv
 3379               		.section	.text._ZdlPv,"ax",@progbits
 3380               	.global	_ZdlPv
 3381               		.type	_ZdlPv, @function
 3382               	_ZdlPv:
 3383               	/* prologue: function */
 3384               	/* frame size = 0 */
 3385 0000 0E94 0000 		call free
 3386               	/* epilogue start */
 3387 0004 0895      		ret
 3388               		.size	_ZdlPv, .-_ZdlPv
 3389               		.section	.text._Znaj,"ax",@progbits
 3390               	.global	_Znaj
 3391               		.type	_Znaj, @function
 3392               	_Znaj:
 3393               	/* prologue: function */
 3394               	/* frame size = 0 */
 3395 0000 0E94 0000 		call malloc
 3396               	/* epilogue start */
 3397 0004 0895      		ret
 3398               		.size	_Znaj, .-_Znaj
 3399               		.section	.text._Znwj,"ax",@progbits
 3400               	.global	_Znwj
 3401               		.type	_Znwj, @function
 3402               	_Znwj:
 3403               	/* prologue: function */
 3404               	/* frame size = 0 */
 3405 0000 0E94 0000 		call malloc
 3406               	/* epilogue start */
 3407 0004 0895      		ret
 3408               		.size	_Znwj, .-_Znwj
 3409               	.global	INT0_handler
 3410               	.global	INT0_handler
 3411               		.section .bss
 3412               		.type	INT0_handler, @object
 3413               		.size	INT0_handler, 4
 3414               	INT0_handler:
 3415 0000 0000 0000 		.skip 4,0
 3416               	.global	INT1_handler
 3417               	.global	INT1_handler
 3418               		.type	INT1_handler, @object
 3419               		.size	INT1_handler, 4
 3420               	INT1_handler:
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 61


 3421 0004 0000 0000 		.skip 4,0
 3422               	.global	INT2_handler
 3423               	.global	INT2_handler
 3424               		.type	INT2_handler, @object
 3425               		.size	INT2_handler, 4
 3426               	INT2_handler:
 3427 0008 0000 0000 		.skip 4,0
 3428               	.global	INT3_handler
 3429               	.global	INT3_handler
 3430               		.type	INT3_handler, @object
 3431               		.size	INT3_handler, 4
 3432               	INT3_handler:
 3433 000c 0000 0000 		.skip 4,0
 3434               	.global	INT4_handler
 3435               	.global	INT4_handler
 3436               		.type	INT4_handler, @object
 3437               		.size	INT4_handler, 4
 3438               	INT4_handler:
 3439 0010 0000 0000 		.skip 4,0
 3440               	.global	INT5_handler
 3441               	.global	INT5_handler
 3442               		.type	INT5_handler, @object
 3443               		.size	INT5_handler, 4
 3444               	INT5_handler:
 3445 0014 0000 0000 		.skip 4,0
 3446               	.global	INT6_handler
 3447               	.global	INT6_handler
 3448               		.type	INT6_handler, @object
 3449               		.size	INT6_handler, 4
 3450               	INT6_handler:
 3451 0018 0000 0000 		.skip 4,0
 3452               	.global	INT7_handler
 3453               	.global	INT7_handler
 3454               		.type	INT7_handler, @object
 3455               		.size	INT7_handler, 4
 3456               	INT7_handler:
 3457 001c 0000 0000 		.skip 4,0
 3458               	.global	TIMER2_COMP_handler
 3459               	.global	TIMER2_COMP_handler
 3460               		.type	TIMER2_COMP_handler, @object
 3461               		.size	TIMER2_COMP_handler, 4
 3462               	TIMER2_COMP_handler:
 3463 0020 0000 0000 		.skip 4,0
 3464               	.global	TIMER2_OVF_handler
 3465               	.global	TIMER2_OVF_handler
 3466               		.type	TIMER2_OVF_handler, @object
 3467               		.size	TIMER2_OVF_handler, 4
 3468               	TIMER2_OVF_handler:
 3469 0024 0000 0000 		.skip 4,0
 3470               	.global	TIMER1_CAPT_handler
 3471               	.global	TIMER1_CAPT_handler
 3472               		.type	TIMER1_CAPT_handler, @object
 3473               		.size	TIMER1_CAPT_handler, 4
 3474               	TIMER1_CAPT_handler:
 3475 0028 0000 0000 		.skip 4,0
 3476               	.global	TIMER1_COMPA_handler
 3477               	.global	TIMER1_COMPA_handler
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 62


 3478               		.type	TIMER1_COMPA_handler, @object
 3479               		.size	TIMER1_COMPA_handler, 4
 3480               	TIMER1_COMPA_handler:
 3481 002c 0000 0000 		.skip 4,0
 3482               	.global	TIMER1_COMPB_handler
 3483               	.global	TIMER1_COMPB_handler
 3484               		.type	TIMER1_COMPB_handler, @object
 3485               		.size	TIMER1_COMPB_handler, 4
 3486               	TIMER1_COMPB_handler:
 3487 0030 0000 0000 		.skip 4,0
 3488               	.global	TIMER1_COMPC_handler
 3489               	.global	TIMER1_COMPC_handler
 3490               		.type	TIMER1_COMPC_handler, @object
 3491               		.size	TIMER1_COMPC_handler, 4
 3492               	TIMER1_COMPC_handler:
 3493 0034 0000 0000 		.skip 4,0
 3494               	.global	TIMER1_OVF_handler
 3495               	.global	TIMER1_OVF_handler
 3496               		.type	TIMER1_OVF_handler, @object
 3497               		.size	TIMER1_OVF_handler, 4
 3498               	TIMER1_OVF_handler:
 3499 0038 0000 0000 		.skip 4,0
 3500               	.global	TIMER0_COMP_handler
 3501               	.global	TIMER0_COMP_handler
 3502               		.type	TIMER0_COMP_handler, @object
 3503               		.size	TIMER0_COMP_handler, 4
 3504               	TIMER0_COMP_handler:
 3505 003c 0000 0000 		.skip 4,0
 3506               	.global	TIMER0_OVF_handler
 3507               	.global	TIMER0_OVF_handler
 3508               		.type	TIMER0_OVF_handler, @object
 3509               		.size	TIMER0_OVF_handler, 4
 3510               	TIMER0_OVF_handler:
 3511 0040 0000 0000 		.skip 4,0
 3512               	.global	CANIT_handler
 3513               	.global	CANIT_handler
 3514               		.type	CANIT_handler, @object
 3515               		.size	CANIT_handler, 4
 3516               	CANIT_handler:
 3517 0044 0000 0000 		.skip 4,0
 3518               	.global	OVRIT_handler
 3519               	.global	OVRIT_handler
 3520               		.type	OVRIT_handler, @object
 3521               		.size	OVRIT_handler, 4
 3522               	OVRIT_handler:
 3523 0048 0000 0000 		.skip 4,0
 3524               	.global	SPI_STC_handler
 3525               	.global	SPI_STC_handler
 3526               		.type	SPI_STC_handler, @object
 3527               		.size	SPI_STC_handler, 4
 3528               	SPI_STC_handler:
 3529 004c 0000 0000 		.skip 4,0
 3530               	.global	USART0_RX_handler
 3531               	.global	USART0_RX_handler
 3532               		.type	USART0_RX_handler, @object
 3533               		.size	USART0_RX_handler, 4
 3534               	USART0_RX_handler:
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 63


 3535 0050 0000 0000 		.skip 4,0
 3536               	.global	USART0_UDRE_handler
 3537               	.global	USART0_UDRE_handler
 3538               		.type	USART0_UDRE_handler, @object
 3539               		.size	USART0_UDRE_handler, 4
 3540               	USART0_UDRE_handler:
 3541 0054 0000 0000 		.skip 4,0
 3542               	.global	USART0_TX_handler
 3543               	.global	USART0_TX_handler
 3544               		.type	USART0_TX_handler, @object
 3545               		.size	USART0_TX_handler, 4
 3546               	USART0_TX_handler:
 3547 0058 0000 0000 		.skip 4,0
 3548               	.global	ANALOG_COMP_handler
 3549               	.global	ANALOG_COMP_handler
 3550               		.type	ANALOG_COMP_handler, @object
 3551               		.size	ANALOG_COMP_handler, 4
 3552               	ANALOG_COMP_handler:
 3553 005c 0000 0000 		.skip 4,0
 3554               	.global	ADC_handler
 3555               	.global	ADC_handler
 3556               		.type	ADC_handler, @object
 3557               		.size	ADC_handler, 4
 3558               	ADC_handler:
 3559 0060 0000 0000 		.skip 4,0
 3560               	.global	EE_READY_handler
 3561               	.global	EE_READY_handler
 3562               		.type	EE_READY_handler, @object
 3563               		.size	EE_READY_handler, 4
 3564               	EE_READY_handler:
 3565 0064 0000 0000 		.skip 4,0
 3566               	.global	TIMER3_CAPT_handler
 3567               	.global	TIMER3_CAPT_handler
 3568               		.type	TIMER3_CAPT_handler, @object
 3569               		.size	TIMER3_CAPT_handler, 4
 3570               	TIMER3_CAPT_handler:
 3571 0068 0000 0000 		.skip 4,0
 3572               	.global	TIMER3_COMPA_handler
 3573               	.global	TIMER3_COMPA_handler
 3574               		.type	TIMER3_COMPA_handler, @object
 3575               		.size	TIMER3_COMPA_handler, 4
 3576               	TIMER3_COMPA_handler:
 3577 006c 0000 0000 		.skip 4,0
 3578               	.global	TIMER3_COMPB_handler
 3579               	.global	TIMER3_COMPB_handler
 3580               		.type	TIMER3_COMPB_handler, @object
 3581               		.size	TIMER3_COMPB_handler, 4
 3582               	TIMER3_COMPB_handler:
 3583 0070 0000 0000 		.skip 4,0
 3584               	.global	TIMER3_COMPC_handler
 3585               	.global	TIMER3_COMPC_handler
 3586               		.type	TIMER3_COMPC_handler, @object
 3587               		.size	TIMER3_COMPC_handler, 4
 3588               	TIMER3_COMPC_handler:
 3589 0074 0000 0000 		.skip 4,0
 3590               	.global	TIMER3_OVF_handler
 3591               	.global	TIMER3_OVF_handler
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 64


 3592               		.type	TIMER3_OVF_handler, @object
 3593               		.size	TIMER3_OVF_handler, 4
 3594               	TIMER3_OVF_handler:
 3595 0078 0000 0000 		.skip 4,0
 3596               	.global	USART1_RX_handler
 3597               	.global	USART1_RX_handler
 3598               		.type	USART1_RX_handler, @object
 3599               		.size	USART1_RX_handler, 4
 3600               	USART1_RX_handler:
 3601 007c 0000 0000 		.skip 4,0
 3602               	.global	USART1_UDRE_handler
 3603               	.global	USART1_UDRE_handler
 3604               		.type	USART1_UDRE_handler, @object
 3605               		.size	USART1_UDRE_handler, 4
 3606               	USART1_UDRE_handler:
 3607 0080 0000 0000 		.skip 4,0
 3608               	.global	USART1_TX_handler
 3609               	.global	USART1_TX_handler
 3610               		.type	USART1_TX_handler, @object
 3611               		.size	USART1_TX_handler, 4
 3612               	USART1_TX_handler:
 3613 0084 0000 0000 		.skip 4,0
 3614               	.global	TWI_handler
 3615               	.global	TWI_handler
 3616               		.type	TWI_handler, @object
 3617               		.size	TWI_handler, 4
 3618               	TWI_handler:
 3619 0088 0000 0000 		.skip 4,0
 3620               	.global	SPM_READY_handler
 3621               	.global	SPM_READY_handler
 3622               		.type	SPM_READY_handler, @object
 3623               		.size	SPM_READY_handler, 4
 3624               	SPM_READY_handler:
 3625 008c 0000 0000 		.skip 4,0
 3626               	.global	dispatcher
 3627               	.global	dispatcher
 3628               		.type	dispatcher, @object
 3629               		.size	dispatcher, 1538
 3630               	dispatcher:
 3631 0090 0000 0000 		.skip 1538,0
 3631      0000 0000 
 3631      0000 0000 
 3631      0000 0000 
 3631      0000 0000 
 3632               	.global	ID
 3633               		.section	.id,"aw",@progbits
 3634               		.type	ID, @object
 3635               		.size	ID, 16
 3636               	ID:
 3637 0000 0000      		.word	0
 3638 0002 0000      		.word	0
 3639 0004 00        		.byte	0
 3640 0005 03        		.byte	3
 3641 0006 19        		.byte	25
 3642 0007 11        		.byte	17
 3643 0008 01        		.byte	1
 3644 0009 0100      		.word	1
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 65


 3645 000b 002F      		.word	12032
 3646 000d DF        		.byte	-33
 3647 000e D9        		.byte	-39
 3648 000f FD        		.byte	-3
 3649               	.global	eeprom
 3650               		.section	.eeprom,"aw",@progbits
 3651               		.type	eeprom, @object
 3652               		.size	eeprom, 32
 3653               	eeprom:
 3654 0000 0000 0000 		.skip 32,0
 3654      0000 0000 
 3654      0000 0000 
 3654      0000 0000 
 3654      0000 0000 
 3655               	.global	data
 3656               	.global	data
 3657               		.section .bss
 3658               		.type	data, @object
 3659               		.size	data, 29
 3660               	data:
 3661 0692 0000 0000 		.skip 29,0
 3661      0000 0000 
 3661      0000 0000 
 3661      0000 0000 
 3661      0000 0000 
 3662               	.global	com
 3663               	.global	com
 3664               		.type	com, @object
 3665               		.size	com, 8
 3666               	com:
 3667 06af 0000 0000 		.skip 8,0
 3667      0000 0000 
 3668               		.weak	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
 3669               		.data
 3670               		.type	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
 3671               		.size	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8Reg
 3672               	_ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8RegisterIh
 3673 0000 00        		.byte	0
 3674 0001 1F        		.byte	31
 3675 0002 2E        		.byte	46
 3676 0003 31        		.byte	49
 3677 0004 4D        		.byte	77
 3678 0005 52        		.byte	82
 3679 0006 63        		.byte	99
 3680 0007 7C        		.byte	124
 3681 0008 8C        		.byte	-116
 3682 0009 93        		.byte	-109
 3683 000a A2        		.byte	-94
 3684 000b BD        		.byte	-67
 3685 000c C1        		.byte	-63
 3686 000d DE        		.byte	-34
 3687 000e EF        		.byte	-17
 3688 000f F0        		.byte	-16
 3689 0010 0B        		.byte	11
 3690 0011 14        		.byte	20
 3691 0012 25        		.byte	37
 3692 0013 3A        		.byte	58
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 66


 3693 0014 46        		.byte	70
 3694 0015 59        		.byte	89
 3695 0016 68        		.byte	104
 3696 0017 77        		.byte	119
 3697 0018 87        		.byte	-121
 3698 0019 98        		.byte	-104
 3699 001a A9        		.byte	-87
 3700 001b B6        		.byte	-74
 3701 001c CA        		.byte	-54
 3702 001d D5        		.byte	-43
 3703 001e E4        		.byte	-28
 3704 001f FB        		.byte	-5
 3705               		.weak	_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEENS1_IN
 3706               		.section	.text,"aw",@progbits
 3707               		.type	_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEENS1_IN
 3708               		.size	_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEENS1_IN
 3709               	_ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEENS1_INS2_ILi3
 3710 0000 00        		.byte	0
 3711 0001 00        		.byte	0
 3712 0002 00        		.byte	0
 3713 0003 00        		.byte	0
 3714 0004 00        		.byte	0
 3715 0005 00        		.byte	0
 3716 0006 00        		.byte	0
 3717 0007 00        		.byte	0
 3718 0008 88        		.byte	-120
 3719 0009 87        		.byte	-121
 3720 000a 86        		.byte	-122
 3721 000b 85        		.byte	-123
 3722 000c 04        		.byte	4
 3723 000d 03        		.byte	3
 3724 000e 02        		.byte	2
 3725 000f 01        		.byte	1
 3726 0010 00        		.byte	0
 3727 0011 00        		.byte	0
 3728 0012 00        		.byte	0
 3729 0013 00        		.byte	0
 3730 0014 00        		.byte	0
 3731 0015 00        		.byte	0
 3732 0016 00        		.byte	0
 3733 0017 00        		.byte	0
 3734 0018 00        		.byte	0
 3735 0019 00        		.byte	0
 3736 001a 00        		.byte	0
 3737 001b 00        		.byte	0
 3738 001c 00        		.byte	0
 3739 001d 00        		.byte	0
 3740 001e 00        		.byte	0
 3741 001f 00        		.byte	0
 3742 0020 00        		.byte	0
 3743 0021 00        		.byte	0
 3744 0022 00        		.byte	0
 3745 0023 00        		.byte	0
 3746 0024 00        		.byte	0
 3747 0025 00        		.byte	0
 3748 0026 00        		.byte	0
 3749 0027 00        		.byte	0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 67


 3750 0028 00        		.byte	0
 3751 0029 00        		.byte	0
 3752 002a 00        		.byte	0
 3753 002b 00        		.byte	0
 3754 002c 00        		.byte	0
 3755 002d 00        		.byte	0
 3756 002e 00        		.byte	0
 3757 002f 00        		.byte	0
 3758 0030 00        		.byte	0
 3759 0031 00        		.byte	0
 3760 0032 00        		.byte	0
 3761 0033 00        		.byte	0
 3762 0034 00        		.byte	0
 3763 0035 00        		.byte	0
 3764 0036 00        		.byte	0
 3765 0037 00        		.byte	0
 3766 0038 00        		.byte	0
 3767 0039 00        		.byte	0
 3768 003a 00        		.byte	0
 3769 003b 00        		.byte	0
 3770 003c 00        		.byte	0
 3771 003d 00        		.byte	0
 3772 003e 00        		.byte	0
 3773 003f 00        		.byte	0
 3774 0040 00        		.byte	0
 3775 0041 00        		.byte	0
 3776 0042 00        		.byte	0
 3777 0043 00        		.byte	0
 3778 0044 00        		.byte	0
 3779 0045 00        		.byte	0
 3780 0046 00        		.byte	0
 3781 0047 00        		.byte	0
 3782 0048 00        		.byte	0
 3783 0049 00        		.byte	0
 3784 004a 00        		.byte	0
 3785 004b 00        		.byte	0
 3786 004c 00        		.byte	0
 3787 004d 00        		.byte	0
 3788 004e 00        		.byte	0
 3789 004f 00        		.byte	0
 3790 0050 00        		.byte	0
 3791 0051 00        		.byte	0
 3792 0052 00        		.byte	0
 3793 0053 00        		.byte	0
 3794 0054 00        		.byte	0
 3795 0055 00        		.byte	0
 3796 0056 00        		.byte	0
 3797 0057 00        		.byte	0
 3798 0058 00        		.byte	0
 3799 0059 00        		.byte	0
 3800 005a 00        		.byte	0
 3801 005b 00        		.byte	0
 3802 005c 00        		.byte	0
 3803 005d 00        		.byte	0
 3804 005e 00        		.byte	0
 3805 005f 00        		.byte	0
 3806 0060 00        		.byte	0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 68


 3807 0061 00        		.byte	0
 3808 0062 00        		.byte	0
 3809 0063 00        		.byte	0
 3810 0064 00        		.byte	0
 3811 0065 00        		.byte	0
 3812 0066 00        		.byte	0
 3813 0067 00        		.byte	0
 3814 0068 00        		.byte	0
 3815 0069 00        		.byte	0
 3816 006a 00        		.byte	0
 3817 006b 00        		.byte	0
 3818 006c 00        		.byte	0
 3819 006d 00        		.byte	0
 3820 006e 00        		.byte	0
 3821 006f 00        		.byte	0
 3822 0070 00        		.byte	0
 3823 0071 00        		.byte	0
 3824 0072 00        		.byte	0
 3825 0073 00        		.byte	0
 3826 0074 00        		.byte	0
 3827 0075 00        		.byte	0
 3828 0076 00        		.byte	0
 3829 0077 00        		.byte	0
 3830 0078 00        		.byte	0
 3831 0079 00        		.byte	0
 3832 007a 00        		.byte	0
 3833 007b 00        		.byte	0
 3834 007c 00        		.byte	0
 3835 007d 00        		.byte	0
 3836 007e 00        		.byte	0
 3837 007f 00        		.byte	0
 3838 0080 00        		.byte	0
 3839 0081 00        		.byte	0
 3840 0082 00        		.byte	0
 3841 0083 00        		.byte	0
 3842 0084 00        		.byte	0
 3843 0085 00        		.byte	0
 3844 0086 00        		.byte	0
 3845 0087 00        		.byte	0
 3846 0088 00        		.byte	0
 3847 0089 00        		.byte	0
 3848 008a 00        		.byte	0
 3849 008b 00        		.byte	0
 3850 008c 00        		.byte	0
 3851 008d 00        		.byte	0
 3852 008e 00        		.byte	0
 3853 008f 00        		.byte	0
 3854 0090 00        		.byte	0
 3855 0091 00        		.byte	0
 3856 0092 00        		.byte	0
 3857 0093 00        		.byte	0
 3858 0094 00        		.byte	0
 3859 0095 00        		.byte	0
 3860 0096 00        		.byte	0
 3861 0097 00        		.byte	0
 3862 0098 00        		.byte	0
 3863 0099 00        		.byte	0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 69


 3864 009a 00        		.byte	0
 3865 009b 00        		.byte	0
 3866 009c 00        		.byte	0
 3867 009d 00        		.byte	0
 3868 009e 00        		.byte	0
 3869 009f 00        		.byte	0
 3870 00a0 00        		.byte	0
 3871 00a1 00        		.byte	0
 3872 00a2 00        		.byte	0
 3873 00a3 00        		.byte	0
 3874 00a4 00        		.byte	0
 3875 00a5 00        		.byte	0
 3876 00a6 00        		.byte	0
 3877 00a7 00        		.byte	0
 3878 00a8 00        		.byte	0
 3879 00a9 00        		.byte	0
 3880 00aa 00        		.byte	0
 3881 00ab 00        		.byte	0
 3882 00ac 00        		.byte	0
 3883 00ad 00        		.byte	0
 3884 00ae 00        		.byte	0
 3885 00af 00        		.byte	0
 3886 00b0 00        		.byte	0
 3887 00b1 00        		.byte	0
 3888 00b2 00        		.byte	0
 3889 00b3 00        		.byte	0
 3890 00b4 00        		.byte	0
 3891 00b5 00        		.byte	0
 3892 00b6 00        		.byte	0
 3893 00b7 00        		.byte	0
 3894 00b8 00        		.byte	0
 3895 00b9 00        		.byte	0
 3896 00ba 00        		.byte	0
 3897 00bb 00        		.byte	0
 3898 00bc 00        		.byte	0
 3899 00bd 00        		.byte	0
 3900 00be 00        		.byte	0
 3901 00bf 00        		.byte	0
 3902 00c0 00        		.byte	0
 3903 00c1 00        		.byte	0
 3904 00c2 00        		.byte	0
 3905 00c3 00        		.byte	0
 3906 00c4 00        		.byte	0
 3907 00c5 00        		.byte	0
 3908 00c6 00        		.byte	0
 3909 00c7 00        		.byte	0
 3910 00c8 00        		.byte	0
 3911 00c9 00        		.byte	0
 3912 00ca 00        		.byte	0
 3913 00cb 00        		.byte	0
 3914 00cc 00        		.byte	0
 3915 00cd 00        		.byte	0
 3916 00ce 00        		.byte	0
 3917 00cf 00        		.byte	0
 3918 00d0 00        		.byte	0
 3919 00d1 00        		.byte	0
 3920 00d2 00        		.byte	0
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 70


 3921 00d3 00        		.byte	0
 3922 00d4 00        		.byte	0
 3923 00d5 00        		.byte	0
 3924 00d6 00        		.byte	0
 3925 00d7 00        		.byte	0
 3926 00d8 00        		.byte	0
 3927 00d9 00        		.byte	0
 3928 00da 00        		.byte	0
 3929 00db 00        		.byte	0
 3930 00dc 00        		.byte	0
 3931 00dd 00        		.byte	0
 3932 00de 00        		.byte	0
 3933 00df 00        		.byte	0
 3934 00e0 00        		.byte	0
 3935 00e1 00        		.byte	0
 3936 00e2 00        		.byte	0
 3937 00e3 00        		.byte	0
 3938 00e4 00        		.byte	0
 3939 00e5 00        		.byte	0
 3940 00e6 00        		.byte	0
 3941 00e7 00        		.byte	0
 3942 00e8 00        		.byte	0
 3943 00e9 00        		.byte	0
 3944 00ea 00        		.byte	0
 3945 00eb 00        		.byte	0
 3946 00ec 00        		.byte	0
 3947 00ed 00        		.byte	0
 3948 00ee 00        		.byte	0
 3949 00ef 00        		.byte	0
 3950 00f0 00        		.byte	0
 3951 00f1 00        		.byte	0
 3952 00f2 00        		.byte	0
 3953 00f3 00        		.byte	0
 3954 00f4 00        		.byte	0
 3955 00f5 00        		.byte	0
 3956 00f6 00        		.byte	0
 3957 00f7 00        		.byte	0
 3958 00f8 00        		.byte	0
 3959 00f9 00        		.byte	0
 3960 00fa 00        		.byte	0
 3961 00fb 00        		.byte	0
 3962 00fc 00        		.byte	0
 3963 00fd 00        		.byte	0
 3964 00fe 00        		.byte	0
 3965 00ff 00        		.byte	0
 3966               		.weak	_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE
 3967               		.type	_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE, @object
 3968               		.size	_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE, 16
 3969               	_ZN9ReIndex16IN4Loki8NullTypeEE5arrayE:
 3970 0100 0000 0000 		.skip 16,0
 3970      0000 0000 
 3970      0000 0000 
 3970      0000 0000 
 3971               	.global __do_copy_data
 3972               	.global __do_clear_bss
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 71


DEFINED SYMBOLS
                            *ABS*:00000000 main.cpp
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2      *ABS*:0000003f __SREG__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3      *ABS*:0000003e __SP_H__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:4      *ABS*:0000003d __SP_L__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:5      *ABS*:00000034 __CCP__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:6      *ABS*:00000000 __tmp_reg__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:7      *ABS*:00000001 __zero_reg__
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:11     .text.__vector_1:00000000 __vector_1
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3414   .bss:00000000 INT0_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:62     .text.__vector_2:00000000 __vector_2
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3420   .bss:00000004 INT1_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:113    .text.__vector_3:00000000 __vector_3
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3426   .bss:00000008 INT2_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:164    .text.__vector_4:00000000 __vector_4
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3432   .bss:0000000c INT3_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:215    .text.__vector_5:00000000 __vector_5
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3438   .bss:00000010 INT4_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:266    .text.__vector_6:00000000 __vector_6
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3444   .bss:00000014 INT5_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:317    .text.__vector_7:00000000 __vector_7
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3450   .bss:00000018 INT6_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:368    .text.__vector_8:00000000 __vector_8
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3456   .bss:0000001c INT7_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:419    .text.__vector_9:00000000 __vector_9
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3462   .bss:00000020 TIMER2_COMP_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:470    .text.__vector_10:00000000 __vector_10
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3468   .bss:00000024 TIMER2_OVF_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:521    .text.__vector_11:00000000 __vector_11
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3474   .bss:00000028 TIMER1_CAPT_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:572    .text.__vector_12:00000000 __vector_12
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3480   .bss:0000002c TIMER1_COMPA_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:623    .text.__vector_13:00000000 __vector_13
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3486   .bss:00000030 TIMER1_COMPB_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:674    .text.__vector_14:00000000 __vector_14
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3492   .bss:00000034 TIMER1_COMPC_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:725    .text.__vector_15:00000000 __vector_15
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3498   .bss:00000038 TIMER1_OVF_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:776    .text.__vector_16:00000000 __vector_16
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3504   .bss:0000003c TIMER0_COMP_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:827    .text.__vector_17:00000000 __vector_17
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3510   .bss:00000040 TIMER0_OVF_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:878    .text.__vector_18:00000000 __vector_18
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3516   .bss:00000044 CANIT_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:929    .text.__vector_19:00000000 __vector_19
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3522   .bss:00000048 OVRIT_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:980    .text.__vector_20:00000000 __vector_20
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3528   .bss:0000004c SPI_STC_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1031   .text.__vector_21:00000000 __vector_21
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3534   .bss:00000050 USART0_RX_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1082   .text.__vector_22:00000000 __vector_22
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3540   .bss:00000054 USART0_UDRE_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1133   .text.__vector_23:00000000 __vector_23
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3546   .bss:00000058 USART0_TX_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1184   .text.__vector_24:00000000 __vector_24
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3552   .bss:0000005c ANALOG_COMP_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1235   .text.__vector_25:00000000 __vector_25
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 72


C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3558   .bss:00000060 ADC_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1286   .text.__vector_26:00000000 __vector_26
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3564   .bss:00000064 EE_READY_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1337   .text.__vector_27:00000000 __vector_27
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3570   .bss:00000068 TIMER3_CAPT_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1388   .text.__vector_28:00000000 __vector_28
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3576   .bss:0000006c TIMER3_COMPA_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1439   .text.__vector_29:00000000 __vector_29
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3582   .bss:00000070 TIMER3_COMPB_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1490   .text.__vector_30:00000000 __vector_30
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3588   .bss:00000074 TIMER3_COMPC_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1541   .text.__vector_31:00000000 __vector_31
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3594   .bss:00000078 TIMER3_OVF_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1592   .text.__vector_32:00000000 __vector_32
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3600   .bss:0000007c USART1_RX_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1643   .text.__vector_33:00000000 __vector_33
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3606   .bss:00000080 USART1_UDRE_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1694   .text.__vector_34:00000000 __vector_34
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3612   .bss:00000084 USART1_TX_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1745   .text.__vector_35:00000000 __vector_35
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3618   .bss:00000088 TWI_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1796   .text.__vector_36:00000000 __vector_36
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3624   .bss:0000008c SPM_READY_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1847   .text.__cxa_guard_acquire:00000000 __cxa_guard_acquire
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1866   .text.__cxa_guard_release:00000000 __cxa_guard_release
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1878   .text.__cxa_guard_abort:00000000 __cxa_guard_abort
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1887   .text.__cxa_pure_virtual:00000000 __cxa_pure_virtual
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:1896   .text._Z3powmm:00000000 _Z3powmm
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2030   .init5:00000000 _Z4Initv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2042   .text.main:00000000 main
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3666   .bss:000006af com
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3630   .bss:00000090 dispatcher
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2101   .text._ZN6srutil8DelegateIFvvEE14empty_functionEPv:00000000 _ZN6srutil8DelegateIFvvEE14empty_functionEPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2110   .text._ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8RegisterIhELj206EES1_I3PinILh2EELj41EES1_ISD_ILh3EELj41EES1_ISD_ILh4EELj41EES1_ISD_ILh7EELj35EELh1ENS_3DatIN4Loki8TypelistINSN_8Int2TypeILi12EEENSO_INSP_ILi13EEENSO_INSP_ILi14EEENSO_INSP_ILi15EEENSN_8NullTypeEEEEEEEEENSO_INSP_ILi8EEENSO_INSP_ILi9EEENSO_INSP_ILi10EEENSO_INSP_ILi11EEESU_EEEEEEEESU_EELZ4dataEE9rxHandlerEv:00000000 _ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8RegisterIhELj206EES1_I3PinILh2EELj41EES1_ISD_ILh3EELj41EES1_ISD_ILh4EELj41EES1_ISD_ILh7EELj35EELh1ENS_3DatIN4Loki8TypelistINSN_8Int2TypeILi12EEENSO_INSP_ILi13EEENSO_INSP_ILi14EEENSO_INSP_ILi15EEENSN_8NullTypeEEEEEEEEENSO_INSP_ILi8EEENSO_INSP_ILi9EEENSO_INSP_ILi10EEENSO_INSP_ILi11EEESU_EEEEEEEESU_EELZ4dataEE9rxHandlerEv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3672   .data:00000000 _ZN4Saut3ComI6HolderI5UcsraLj200EES1_I5UcsrbLj201EES1_I5UcsrcLj202EES1_I4UbrrLj204EES1_I8RegisterIhELj206EES1_I3PinILh2EELj41EES1_ISD_ILh3EELj41EES1_ISD_ILh4EELj41EES1_ISD_ILh7EELj35EELh1ENS_3DatIN4Loki8TypelistINSN_8Int2TypeILi12EEENSO_INSP_ILi13EEENSO_INSP_ILi14EEENSO_INSP_ILi15EEENSN_8NullTypeEEEEEEEEENSO_INSP_ILi8EEENSO_INSP_ILi9EEENSO_INSP_ILi10EEENSO_INSP_ILi11EEESU_EEEEEEEESU_EELZ4dataEE3hamE
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3709   .text:00000000 _ZN10ReIndex256IN4Loki8TypelistINS0_8Int2TypeILi12EEENS1_INS2_ILi4EEENS1_INS2_ILi13EEENS1_INS2_ILi3EEENS1_INS2_ILi14EEENS1_INS2_ILi2EEENS1_INS2_ILi15EEENS1_INS2_ILi1EEENS1_INS2_ILi8EEENS1_INS2_ILi136EEENS1_INS2_ILi9EEENS1_INS2_ILi135EEENS1_INS2_ILi10EEENS1_INS2_ILi134EEENS1_INS2_ILi11EEENS1_INS2_ILi133EEENS0_8NullTypeEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE5arrayE
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3660   .bss:00000692 data
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3969   .text:00000100 _ZN9ReIndex16IN4Loki8NullTypeEE5arrayE
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2625   .text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_9rxHandlerEvEEEEvPv:00000000 _ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_9rxHandlerEvEEEEvPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2635   .text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_9txHandlerEvEEEEvPv:00000000 _ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_9txHandlerEvEEEEvPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2661   .text._ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_11udreHandlerEvEEEEvPv:00000000 _ZN6srutil8DelegateIFvvEE11method_stubIN4Saut3ComI6HolderI5UcsraLj200EES6_I5UcsrbLj201EES6_I5UcsrcLj202EES6_I4UbrrLj204EES6_I8RegisterIhELj206EES6_I3PinILh2EELj41EES6_ISI_ILh3EELj41EES6_ISI_ILh4EELj41EES6_ISI_ILh7EELj35EELh1ENS4_3DatIN4Loki8TypelistINSS_8Int2TypeILi12EEENST_INSU_ILi13EEENST_INSU_ILi14EEENST_INSU_ILi15EEENSS_8NullTypeEEEEEEEEENST_INSU_ILi8EEENST_INSU_ILi9EEENST_INSU_ILi10EEENST_INSU_ILi11EEESZ_EEEEEEEESZ_EELZ4dataEEEXadL_ZNS1D_11udreHandlerEvEEEEvPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2750   .text._ZN6srutil8DelegateIFvjEE14empty_functionEPvj:00000000 _ZN6srutil8DelegateIFvjEE14empty_functionEPvj
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:2758   .text._GLOBAL__I_INT0_handler:00000000 _GLOBAL__I_INT0_handler
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3001   .text._Z13commandParserv:00000000 _Z13commandParserv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3636   .id:00000000 ID
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3310   .text._Z8bandDiamPKhS0_:00000000 _Z8bandDiamPKhS0_
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3372   .text._ZdaPv:00000000 _ZdaPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3382   .text._ZdlPv:00000000 _ZdlPv
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3392   .text._Znaj:00000000 _Znaj
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3402   .text._Znwj:00000000 _Znwj
C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s:3653   .eeprom:00000000 eeprom

UNDEFINED SYMBOLS
__muldi3
__do_global_ctors
__eerd_word_can128
__eerd_byte_can128
GAS LISTING C:\DOCUME~1\nazemnyh\LOCALS~1\Temp/ccU1xqp0.s 			page 73


free
malloc
__do_copy_data
__do_clear_bss
