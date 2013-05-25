#
# Makefile for 08gr450 
# Support generation of EPS files from SVG using Inkscape
#

#
# "THE BEER-WARE LICENSE" (Revision 42):
# <08gr450@es.aau.dk> wrote this file. As long as you retain this notice you
# can do whatever you want with this stuff. If we meet some day, and you think
# this stuff is worth it, you can buy me a beer in return.
#

#DIRS
# Where to put the converted eps images
# without / 
EPS_DIR = eps
IMG_SRC_DIR = images
PDFREADER= evince
PDFREADER2= acroread
# All the files to check for changes
SRCS  = $(shell find . -name '*.tex')
SVGS  = $(wildcard images/*.svg)
EPSS  = $(wildcard images/*.eps)
DOTS  = $(wildcard images/*.dot)
DIAS  = $(wildcard images/*.dia)
PNGS  = $(wildcard images/*.png)
JPGS  = $(wildcard images/*.jpg)
GIFS  = $(wildcard images/*.gif)
BMPS  = $(wildcard images/*.bmp)

# All the SVG files which have a DOT version are found
SVGS_DOTS = $(addsuffix .svg, $(basename $(DOTS)))
EPSS_SVGS = $(addprefix $(EPS_DIR)/, $(notdir $(addsuffix .eps, $(basename $(SVGS_DOTS)))))

# All the EPS files which have a SVG version are found
EPSS_SVGS +=  $(addprefix $(EPS_DIR)/, $(notdir $(addsuffix .eps, $(basename $(SVGS)))))

# All the EPS files which have a DIA version are found and change the directory to save the EPS image
EPSS_DIAS_NODIR = $(notdir $(addsuffix .eps, $(basename $(DIAS))))
EPSS_DIAS =   $(addprefix $(EPS_DIR)/, $(EPSS_DIAS_NODIR))

# All the EPS files which have a PNG version are found and change the directory to save the EPS image
EPSS_PNGS_NODIR = $(notdir $(addsuffix .eps, $(basename $(PNGS))))
EPSS_PNGS =   $(addprefix $(EPS_DIR)/, $(EPSS_PNGS_NODIR))

# All the EPS files which have a JPG version are found and change the directory to save the EPS image
EPSS_JPGS_NODIR = $(notdir $(addsuffix .eps, $(basename $(JPGS))))
EPSS_JPGS =   $(addprefix $(EPS_DIR)/, $(EPSS_JPGS_NODIR))

# All the EPS files which have a GIF version are found and change the directory to save the EPS image
EPSS_GIFS_NODIR = $(notdir $(addsuffix .eps, $(basename $(GIFS))))
EPSS_GIFS =   $(addprefix $(EPS_DIR)/, $(EPSS_GIFS_NODIR))

# All the EPS files which have a BMP version are found and change the directory to save the EPS image
EPSS_BMPS_NODIR = $(notdir $(addsuffix .eps, $(basename $(BMPS))))
EPSS_BMPS =   $(addprefix $(EPS_DIR)/, $(EPSS_BMPS_NODIR))


# The main tex file to compile
#MAIN = final_lin
MAIN = report
DRAFT = draft
TARGET = master

#MAIN = master_app
#TARGET = master_app

# The tex compiler
# Mmm.. Rubber is good :)
TEX = rubber -W refs -W misc 
# div. converters
DVI2PS = dvips
PS2PDF = ps2pdf


all: $(MAIN).pdf
	@echo ""
	@echo "** Cleaning up.."
	rm -f */*.aux *.aux
	@echo ""
	@echo "** Done compiling $(MAIN).pdf"

ps: $(MAIN).ps
	@echo ""
	@echo "** Cleaning up.."
	rm -f */*.aux *.aux
	@echo ""
	@echo "** Done compiling $(MAIN).ps"

$(MAIN).pdf: $(MAIN).ps
	@echo ""
	@echo "** Converting $(MAIN).ps to $(TARGET).pdf"
	$(PS2PDF) $(MAIN).ps $(TARGET).pdf
	#cp master.pdf ~/Dropbox/P6/latestreport.pdf

$(MAIN).ps: $(MAIN).dvi
	@echo ""
	@echo "** Converting $(MAIN).dvi to $(MAIN).ps"
	$(DVI2PS) -q -P pdf -G0 -j0 -o $(MAIN).ps $(MAIN).dvi

$(MAIN).dvi: $(MAIN).tex $(SRCS) $(EPSS) $(EPSS_SVGS) $(EPSS_DIAS) $(EPSS_JPGS) $(EPSS_PNGS) $(EPSS_GIFS) $(EPSS_BMPS)
	$(TEX) $(MAIN)


## Make draft
draft: $(DRAFT).pdf
	@echo ""
	@echo "** Cleaning up.."
	rm -f */*.aux *.aux
	@echo ""
	@echo "** Done compiling $(DRAFT).pdf"


$(DRAFT).pdf: $(DRAFT).ps
	@echo ""
	@echo "** Converting $(DRAFT).ps to $(DRAFT).pdf"
	$(PS2PDF) $(DRAFT).ps $(TARGET).pdf

$(DRAFT).ps: $(DRAFT).dvi
	@echo ""
	@echo "** Converting $(DRAFT).dvi to $(DRAFT).ps"
	$(DVI2PS) -q -P pdf -G0 -j0 -o $(DRAFT).ps $(DRAFT).dvi

$(DRAFT).dvi: $(DRAFT).tex $(SRCS) $(EPSS) $(EPSS_SVGS) $(EPSS_DIAS) $(EPSS_JPGS) $(EPSS_PNGS) $(EPSS_GIFS) $(EPSS_BMPS)
	#@rm -f svninfo.tex
	#@svn info >> svninfo.tex	
	$(TEX) -v $(DRAFT)


# Use Dia to convert .dia files to .eps files
#$(EPSS_DIAS): $(DIAS) 
#	@echo ""
#	@echo "** Generating EPS from $<!!"
#	dia $< -t eps -e $@

$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.dia 
	@echo ""
	@echo "** Generating EPS from $<!!"
	dia $< -t eps -e $@

# Use inkscape to convert .svg files to .eps files
$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.svg 
	@echo ""
	@echo "** Generating EPS from $<!!"
	inkscape -t -T --export-eps=$@ $<
	
# Use convert to convert .png files to .eps files
$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.png 
	@echo ""
	@echo "** Generating EPS from $<!!"
	convert $< $@	

# Use convert to convert .bmp files to .eps files
$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.bmp 
	@echo ""
	@echo "** Generating EPS from $<!!"
	convert $< $@	
	
# Use convert to convert .jpg files to .eps files
$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.jpg 
	@echo ""
	@echo "** Generating EPS from $<!!"
	convert $< $@

# Use convert to convert .gif files to .eps files
$(EPS_DIR)/%.eps: $(IMG_SRC_DIR)/%.gif 
	@echo ""
	@echo "** Generating EPS from $<!!"
	convert $< $@

# Use dot to convert .dot files to .svg files
%.svg: %.dot 
	@echo ""
	@echo "** Generating SVG from $<!!"
	dot -Tsvg -o $@ $<

# %.nls: %.nlo
#	makeindex $< -s nomencl.ist -o $@


force: clean all

clean:
	rm -f *.toc *.aux *.pdf *.ps *.log *.lof *.bbl *.blg *.dvi */*.aux *.glo *.gls *.ilg *.nls *.nlo */*.tex~ */*.tex.backup */*/*.m~ *.lox

# A rule for removing all EPS files which have a SVG and DIA version
clean_eps: 
	rm -f $(EPSS_SVGS) 
	rm -f $(EPSS_DIAS)

# A rule for removing all EPS files which have a DOT version
clean_svg: 
	rm -f $(SVGS_DOTS) 

open: $(MAIN).pdf
	@echo ""
	@echo "Opening $(MAIN).pdf"
	${PDFREADER} $(TARGET).pdf
	
kopen: $(MAIN).pdf
	@echo ""
	@echo "Opening $(MAIN).pdf"
	screen -d -m ${PDFREADER} $(TARGET).pdf

psopen: $(MAIN).ps
	@echo ""
	@echo "Opening $(MAIN).ps"
	screen -d -m ${PDFREADER2} final.ps

psopenfull: psopen
	@echo ""
	@echo "** Converting $(MAIN).ps to $(MAIN).pdf"
	$(PS2PDF) $(MAIN).ps
	#cp $(TARGET).pdf ~/Dropbox/P6/latestreport.pdf
	@echo ""
	@echo "Opening $(MAIN).pdf"
	screen -d -m ${PDFREADER2} $(TARGET).pdf

debug: 
	@echo "$(DIAS)"

