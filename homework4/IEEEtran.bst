%%
%% IEEEtran.bst
%% BibTeX Bibliography Style file for IEEE Journals and Conferences (unsorted)
%% Version 1.14 (2015/08/26)
%% 
%% Copyright (c) 2003-2015 Michael Shell
%% 
%% Original starting code base and algorithms obtained from the output of
%% Patrick W. Daly's makebst package as well as from prior versions of
%% IEEE BibTeX styles:
%% 
%% 1. Howard Trickey and Oren Patashnik's ieeetr.bst  (1985/1988)
%% 2. Silvano Balemi and Richard H. Roy's IEEEbib.bst (1993)
%% 
%% Support sites:
%% http://www.michaelshell.org/tex/ieeetran/
%% http://www.ctan.org/pkg/ieeetran
%% and/or
%% http://www.ieee.org/
%% 
%% For use with BibTeX version 0.99a or later
%%
%% This is a numerical citation style.
%% 
%%*************************************************************************
%% Legal Notice:
%% This code is offered as-is without any warranty either expressed or
%% implied; without even the implied warranty of MERCHANTABILITY or
%% FITNESS FOR A PARTICULAR PURPOSE! 
%% User assumes all risk.
%% In no event shall the IEEE or any contributor to this code be liable for
%% any damages or losses, including, but not limited to, incidental,
%% consequential, or any other damages, resulting from the use or misuse
%% of any information contained here.
%%
%% All comments are the opinions of their respective authors and are not
%% necessarily endorsed by the IEEE.
%%
%% This work is distributed under the LaTeX Project Public License (LPPL)
%% ( http://www.latex-project.org/ ) version 1.3, and may be freely used,
%% distributed and modified. A copy of the LPPL, version 1.3, is included
%% in the base LaTeX documentation of all distributions of LaTeX released
%% 2003/12/01 or later.
%% Retain all contribution notices and credits.
%% ** Modified files should be clearly indicated as such, including  **
%% ** renaming them and changing author support contact information. **
%%*************************************************************************


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% DEFAULTS FOR THE CONTROLS OF THE BST STYLE %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% These are the defaults for the user adjustable controls. The values used
% here can be overridden by the user via IEEEtranBSTCTL entry type.

% NOTE: The recommended LaTeX command to invoke a control entry type is:
% 
%\makeatletter
%\def\bstctlcite{\@ifnextchar[{\@bstctlcite}{\@bstctlcite[@auxout]}}
%\def\@bstctlcite[#1]#2{\@bsphack
%  \@for\@citeb:=#2\do{%
%    \edef\@citeb{\expandafter\@firstofone\@citeb}%
%    \if@filesw\immediate\write\csname #1\endcsname{\string\citation{\@citeb}}\fi}%
%  \@esphack}
%\makeatother
%
% It is called at the start of the document, before the first \cite, like:
% \bstctlcite{IEEEexample:BSTcontrol}
%
% IEEEtran.cls V1.6 and later does provide this command.



% #0 turns off the display of the number for articles.
% #1 enables
FUNCTION {default.is.use.number.for.article} { #1 }


% #0 turns off the display of the paper and type fields in @inproceedings.
% #1 enables
FUNCTION {default.is.use.paper} { #1 }


% #0 turns off the display of urls
% #1 enables
FUNCTION {default.is.use.url} { #1 }


% #0 turns off the forced use of "et al."
% #1 enables
FUNCTION {default.is.forced.et.al} { #0 }


% The maximum number of names that can be present beyond which an "et al."
% usage is forced. Be sure that num.names.shown.with.forced.et.al (below)
% is not greater than this value!
% Note: There are many instances of references in IEEE journals which have
% a very large number of authors as well as instances in which "et al." is
% used profusely.
FUNCTION {default.max.num.names.before.forced.et.al} { #10 }


% The number of names that will be shown with a forced "et al.".
% Must be less than or equal to max.num.names.before.forced.et.al
FUNCTION {default.num.names.shown.with.forced.et.al} { #1 }


% #0 turns off the alternate interword spacing for entries with URLs.
% #1 enables
FUNCTION {default.is.use.alt.interword.spacing} { #1 }


% If alternate interword spacing for entries with URLs is enabled, this is
% the interword spacing stretch factor that will be used. For example, the
% default "4" here means that the interword spacing in entries with URLs can
% stretch to four times normal. Does not have to be an integer. Note that
% the value specified here can be overridden by the user in their LaTeX
% code via a command such as: 
% "\providecommand\BIBentryALTinterwordstretchfactor{1.5}" in addition to
% that via the IEEEtranBSTCTL entry type.
FUNCTION {default.ALTinterwordstretchfactor} { "4" }


% #0 turns off the "dashification" of repeated (i.e., identical to those
% of the previous entry) names. The IEEE normally does this.
% #1 enables
FUNCTION {default.is.dash.repeated.names} { #1 }


% The default name format control string.
FUNCTION {default.name.format.string}{ "{f.~}{vv~}{ll}{, jj}" }


% The default LaTeX font command for the names.
FUNCTION {default.name.latex.cmd}{ "" }


% The default URL prefix.
FUNCTION {default.name.url.prefix}{ "[Online]. Available:" }


% Other controls that cannot be accessed via IEEEtranBSTCTL entry type.

% #0 turns off the terminal startup banner/completed message so as to
% operate more quietly.
% #1 enables
FUNCTION {is.print.banners.to.terminal} { #1 }




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% FILE VERSION AND BANNER %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

FUNCTION{bst.file.version} { "1.14" }
FUNCTION{bst.file.date} { "2015/08/26" }
FUNCTION{bst.file.website} { "http://www.michaelshell.org/tex/ieeetran/bibtex/" }

FUNCTION {banner.message}
{ is.print.banners.to.terminal
     { "-- IEEEtran.bst version" " " * bst.file.version *
       " (" * bst.file.date * ") " * "by Michael Shell." *
       top$
       "-- " bst.file.website *
       top$
       "-- See the " quote$ * "IEEEtran_bst_HOWTO.pdf" * quote$ * " manual for usage information." *
       top$
     }
     { skip$ }
   if$
}

FUNCTION {completed.message}
{ is.print.banners.to.terminal
     { ""
       top$
       "Done."
       top$
     }
     { skip$ }
   if$
}




%%%%%%%%%%%%%%%%%%%%%%
%% STRING CONSTANTS %%
%%%%%%%%%%%%%%%%%%%%%%

FUNCTION {bbl.and}{ "and" }
FUNCTION {bbl.etal}{ "et~al." }
FUNCTION {bbl.editors}{ "eds." }
FUNCTION {bbl.editor}{ "ed." }
FUNCTION {bbl.edition}{ "ed." }
FUNCTION {bbl.volume}{ "vol." }
FUNCTION {bbl.of}{ "of" }
FUNCTION {bbl.number}{ "no." }
FUNCTION {bbl.in}{ "in" }
FUNCTION {bbl.pages}{ "pp." }
FUNCTION {bbl.page}{ "p." }
FUNCTION {bbl.chapter}{ "ch." }
FUNCTION {bbl.paper}{ "paper" }
FUNCTION {bbl.part}{ "pt." }
FUNCTION {bbl.patent}{ "Patent" }
FUNCTION {bbl.patentUS}{ "U.S." }
FUNCTION {bbl.revision}{ "Rev." }
FUNCTION {bbl.series}{ "ser." }
FUNCTION {bbl.standard}{ "Std." }
FUNCTION {bbl.techrep}{ "Tech. Rep." }
FUNCTION {bbl.mthesis}{ "Master's thesis" }
FUNCTION {bbl.phdthesis}{ "Ph.D. dissertation" }
FUNCTION {bbl.st}{ "st" }
FUNCTION {bbl.nd}{ "nd" }
FUNCTION {bbl.rd}{ "rd" }
FUNCTION {bbl.th}{ "th" }


% This is the LaTeX spacer that is used when a larger than normal space
% is called for (such as just before the address:publisher).
FUNCTION {large.space} { "\hskip 1em plus 0.5em minus 0.4em\relax " }

% The LaTeX code for dashes that are used to represent repeated names.
% Note: Some older IEEE journals used something like
% "\rule{0.275in}{0.5pt}\," which is fairly thick and runs right along
% the baseline. However, the IEEE now uses a thinner, above baseline,
% six dash long sequence.
FUNCTION {repeated.name.dashes} { "------" }



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% PREDEFINED STRING MACROS %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

MACRO {jan} {"Jan."}
MACRO {feb} {"Feb."}
MACRO {mar} {"Mar."}
MACRO {apr} {"Apr."}
MACRO {may} {"May"}
MACRO {jun} {"Jun."}
MACRO {jul} {"Jul."}
MACRO {aug} {"Aug."}
MACRO {sep} {"Sep."}
MACRO {oct} {"Oct."}
MACRO {nov} {"Nov."}
MACRO {dec} {"Dec."}



%%%%%%%%%%%%%%%%%%
%% ENTRY FIELDS %%
%%%%%%%%%%%%%%%%%%

ENTRY
  { address
    assignee
    author
    booktitle
    chapter
    day
    dayfiled
    edition
    editor
    howpublished
    institution
    intype
    journal
    key
    language
    month
    monthfiled
    nationality
    note
    number
    organization
    pages
    paper
    publisher
    school
    series
    revision
    title
    type
    url
    volume
    year
    yearfiled
    CTLuse_article_number
    CTLuse_paper
    CTLuse_url
    CTLuse_forced_etal
    CTLmax_names_forced_etal
    CTLnames_show_etal
    CTLuse_alt_spacing
    CTLalt_stretch_factor
    CTLdash_repeated_names
    CTLname_format_string
    CTLname_latex_cmd
    CTLname_url_prefix
  }
  {}
  { label }




%%%%%%%%%%%%%%%%%%%%%%%
%% INTEGER VARIABLES %%
%%%%%%%%%%%%%%%%%%%%%%%

INTEGERS { prev.status.punct this.status.punct punct.std
           punct.no punct.comma punct.period 
           prev.status.space this.status.space space.std
           space.no space.normal space.large
           prev.status.quote this.status.quote quote.std
           quote.no quote.close
           prev.status.nline this.status.nline nline.std
           nline.no nline.newblock 
           status.cap cap.std
           cap.no cap.yes}

INTEGERS { longest.label.width multiresult nameptr namesleft number.label numnames }

INTEGERS { is.use.number.for.article
           is.use.paper
           is.use.url
           is.forced.et.al
           max.num.names.before.forced.et.al
           num.names.shown.with.forced.et.al
           is.use.alt.interword.spacing
           is.dash.repeated.names}


%%%%%%%%%%%%%%%%%%%%%%
%% STRING VARIABLES %%
%%%%%%%%%%%%%%%%%%%%%%

STRINGS { bibinfo
          longest.label
          oldname
          s
          t
          ALTinterwordstretchfactor
          name.format.string
          name.latex.cmd
          name.url.prefix}




%%%%%%%%%%%%%%%%%%%%%%%%%
%% LOW LEVEL FUNCTIONS %%
%%%%%%%%%%%%%%%%%%%%%%%%%

FUNCTION {initialize.controls}
{ default.is.use.number.for.article 'is.use.number.for.article :=
  default.is.use.paper 'is.use.paper :=
  default.is.use.url 'is.use.url :=
  default.is.forced.et.al 'is.forced.et.al :=
  default.max.num.names.before.forced.et.al 'max.num.names.before.forced.et.al :=
  default.num.names.shown.with.forced.et.al 'num.names.shown.with.forced.et.al :=
  default.is.use.alt.interword.spacing 'is.use.alt.interword.spacing :=
  default.is.dash.repeated.names 'is.dash.repeated.names :=
  default.ALTinterwordstretchfactor 'ALTinterwordstretchfactor :=
  default.name.format.string 'name.format.string :=
  default.name.latex.cmd 'name.latex.cmd :=
  default.name.url.prefix 'name.url.prefix :=
}


% This IEEEtran.bst features a very powerful and flexible mechanism for
% controlling the capitalization, punctuation, spacing, quotation, and
% newlines of the formatted entry fields. (Note: IEEEtran.bst does not need
% or use the newline/newblock feature, but it has been implemented for
% possible future use.) The output states of IEEEtran.bst consist of
% multiple independent attributes and, as such, can be thought of as being
% vectors, rather than the simple scalar values ("before.all", 
% "mid.sentence", etc.) used in most other .bst files.
% 
% The more flexible and complex design used here was motivated in part by
% the IEEE's rather unusual bibliography style. For example, the IEEE ends the
% previous field item with a period and large space prior to the publisher
% address; the @electronic entry types use periods as inter-item punctuation
% rather than the commas used by the other entry types; and URLs are never
% followed by periods even though they are the last item in the entry.
% Although it is possible to accommodate these features with the conventional
% output state system, the seemingly endless exceptions make for convoluted,
% unreliable and difficult to maintain code.
%
% IEEEtran.bst's output state system can be easily understood via a simple
% illustration of two most recently formatted entry fields (on the stack):
%
%               CURRENT_ITEM
%               "PREVIOUS_ITEM
%
% which, in this example, is to eventually appear in the bibliography as:
% 
%               "PREVIOUS_ITEM," CURRENT_ITEM
%
% It is the job of the output routine to take the previous item off of the
% stack (while leaving the current item at the top of the stack), apply its
% trailing punctuation (including closing quote marks) and spacing, and then
% to write the result to BibTeX's output buffer:
% 
%               "PREVIOUS_ITEM," 
% 
% Punctuation (and spacing) between items is often determined by both of the
% items rather than just the first one. The presence of quotation marks
% further complicates the situation because, in standard English, trailing
% punctuation marks are supposed to be contained within the quotes.
% 
% IEEEtran.bst maintains two output state (aka "status") vectors which
% correspond to the previous and current (aka "this") items. Each vector
% consists of several independent attributes which track punctuation,
% spacing, quotation, and newlines. Capitalization status is handled by a
% separate scalar because the format routines, not the output routine,
% handle capitalization and, therefore, there is no need to maintain the
% capitalization attribute for both the "previous" and "this" items.
% 
% When a format routine adds a new item, it copies the current output status
% vector to the previous output status vector and (usually) resets the
% current (this) output status vector to a "standard status" vector. Using a
% "standard status" vector in this way allows us to redefine what we mean by
% "standard status" at the start of each entry handler and reuse the same
% format routines under the various inter-item separation schemes. For
% example, the standard status vector for the @book entry type may use
% commas for item separators, while the @electronic type may use periods,
% yet both entry handlers exploit many of the exact same format routines.
% 
% Because format routines have write access to the output status vector of
% the previous item, they can override the punctuation choices of the
% previous format routine! Therefore, it becomes trivial to implement rules
% such as "Always use a period and a large space before the publisher." By
% pushing the generation of the closing quote mark to the output routine, we
% avoid all the problems caused by having to close a quote before having all
% the information required to determine what the punctuation should be.
%
% The IEEEtran.bst output state system can easily be expanded if needed.
% For instance, it is easy to add a "space.tie" attribute value if the
% bibliography rules mandate that two items have to be joined with an
% unbreakable space. 

FUNCTION {initialize.status.constants}
{ #0 'punct.no :=
  #1 'punct.comma :=
  #2 'punct.period :=
  #0 'space.no := 
  #1 'space.normal :=
  #2 'space.large :=
  #0 'quote.no :=
  #1 'quote.close :=
  #0 'cap.no :=
  #1 'cap.yes :=
  #0 'nline.no :=
  #1 'nline.newblock :=
}

FUNCTION {std.status.using.comma}
{ punct.comma 'punct.std :=
  space.normal 'space.std :=
  quote.no 'quote.std :=
  nline.no 'nline.std :=
  cap.no 'cap.std :=
}

FUNCTION {std.status.using.period}
{ punct.period 'punct.std :=
  space.normal 'space.std :=
  quote.no 'quote.std :=
  nline.no 'nline.std :=
  cap.yes 'cap.std :=
}

FUNCTION {initialize.prev.this.status}
{ punct.no 'prev.status.punct :=
  space.no 'prev.status.space :=
  quote.no 'prev.status.quote :=
  nline.no 'prev.status.nline :=
  punct.no 'this.status.punct :=
  space.no 'this.status.space :=
  quote.no 'this.status.quote :=
  nline.no 'this.status.nline :=
  cap.yes 'status.cap :=
}

FUNCTION {this.status.std}
{ punct.std 'this.status.punct :=
  space.std 'this.status.space :=
  quote.std 'this.status.quote :=
  nline.std 'this.status.nline :=
}

FUNCTION {cap.status.std}{ cap.std 'status.cap := }

FUNCTION {this.to.prev.status}
{ this.status.punct 'prev.status.punct :=
  this.status.space 'prev.status.space :=
  this.status.quote 'prev.status.quote :=
  this.status.nline 'prev.status.nline :=
}


FUNCTION {not}
{   { #0 }
    { #1 }
  if$
}

FUNCTION {and}
{   { skip$ }
    { pop$ #0 }
  if$
}

FUNCTION {or}
{   { pop$ #1 }
    { skip$ }
  if$
}


% convert the strings "yes" or "no" to #1 or #0 respectively
FUNCTION {yes.no.to.int}
{ "l" change.case$ duplicate$
    "yes" =
    { pop$  #1 }
    { duplicate$ "no" =
        { pop$ #0 }
      