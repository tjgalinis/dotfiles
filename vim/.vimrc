"-------------------------------------------------------------------------------
" File: ~/.vimrc
" Author: Thomas Galinis <tjgalinis@gmail.com>
"-------------------------------------------------------------------------------

" Defaults {{{

set ruler
set showcmd
set showmatch
set ignorecase
set smartcase
set incsearch
set nohlsearch
"set autowrite
"set hidden
"set mouse=a
set ruler
set nowrap
"set number
set relativenumber
set scrolloff=4
set splitright splitbelow
set cursorline
set textwidth=0
set colorcolumn=81
set foldmethod=marker

" For gvim
set guioptions-=m  "remove menu bar
set guioptions-=T  "remove toolbar
set guioptions-=r  "remove right-hand scroll bar
set guioptions-=L  "remove left-hand scroll bar

" }}}
" Indentation {{{

set tabstop=4
set softtabstop=4
set shiftwidth=4 
set expandtab
set smarttab

filetype plugin indent on

" }}}
" Syntax highlighting and colorscheme {{{

syntax on
set background=dark
packadd! onedark.vim
colorscheme onedark
"set t_Co=16
"set colorscheme solarized8

" }}}

