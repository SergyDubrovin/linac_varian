syntax on
set number

" Поиск
set ic
set hls
set is

set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab

set autoindent
set smartindent

set colorcolumn=110
highlight ColorColumn ctermbg=darkgray

augroup project
	autocmd!
	autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END

let &path.="src/include,/usr/include/AL,/home/rocezen/geant4/geant4_install/include/Geant4/,"

let g:ycm_global_ycm_extra_conf="~/.vim/.ycm_extra_conf.py"


