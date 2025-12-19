project = 'CHB Notes and Manual'
author = 'Andres26'
release = '0.05-prealpha'

extensions = [] 
templates_path = ['_templates']
exclude_patterns = []

# HTML GEN
html_theme = 'classic'
html_static_path = ['_static']

# LATEX GEN
latex_engine = 'pdflatex'

latex_documents = [
    (
        'index',
        'chb.tex',
        'CHB Documentation and manual',
        'andres26',
        'manual',
    ),
]

latex_elements = {
    'classoptions': ',openany',
    'documentclass': 'article',
    'papersize': 'a4paper',
    'pointsize': '10pt',
}


# CHM GEN
htmlhelp_basename = 'CHBManual'
